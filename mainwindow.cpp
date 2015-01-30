#include "becreative.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QRegExpValidator *regExpValidator = new QRegExpValidator(QRegExp("[0-9A-F ]*", Qt::CaseInsensitive, QRegExp::RegExp), this);
    ui->editInsertText->setValidator(regExpValidator);
    statusLabel = new QLabel("Ready - No rom loaded...");
    ui->statusBar->addWidget(statusLabel);

    compiler = new Compiler("/home/florian/devkitPRO/devkitARM/bin", this);
    errorDock = new ErrorDock(ui->tblErrors);

    if (!initializeSourceBrowser())
    {
        exit(1);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
QString MainWindow::compilerPath() const
{
    return _compilerPath;
}

void MainWindow::setCompilerPath(const QString &compilerPath)
{
    _compilerPath = compilerPath;
}

QDir MainWindow::directory() const
{
    return _directory;
}

void MainWindow::setDirectory(const QDir &directory)
{
    _directory = directory;
}



int MainWindow::compileFile(QString path)
{
    compiler->setDirectory(path.left(path.lastIndexOf("/")));

    QString file;
    file = path.mid(path.lastIndexOf("/") + 1);

    if (!compiler->compile(file))
    {
        qDebug() << "Failed to compile...";
    }
}

bool MainWindow::initializeSourceBrowser()
{
    setDirectory(QDir::homePath() + "/romhacking");
    directory().setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    QList<QTreeWidgetItem *> files;
    QStringList filters;

    filters << "*.asm";
    directory().setNameFilters(filters);

    foreach(QFileInfo file, directory().entryInfoList())
    {
        if (file.fileName().right(4) == ".asm")
        {
            compileFile(file.absoluteFilePath());
            QFile oFile(file.absoluteFilePath() + ".bin");
            QString size = "0";

            if (oFile.open(QIODevice::ReadOnly))
            {
                size = QString::number(oFile.size());
            }
            else
            {
                size = "-1";
            }

            QStringList infos;
            infos << file.fileName() << size + " Bytes";

            files.append(new QTreeWidgetItem(infos));
        }
    }
    ui->treeSourceFiles->addTopLevelItems(files);

    return true;
}

bool MainWindow::insertOffsets(int minimum)
{
    ui->listOffsets->clear();
    rom->openStream(QIODevice::ReadOnly);

    QList<int> results;
    int freeBytes = 0;
    while (!rom->atEnd())
    {
        char byte = rom->readByte();
        if (byte == -1)
        {
            int count = 1;
            char nextByte = rom->readByte();
            while (nextByte == -1)
            {
                count++;
                nextByte = rom->readByte();
            }
            if (count >= minimum)
            {
                results.append(rom->pos()  - count - 1);
                freeBytes += count;
            }
        }
    }

    rom->closeStream();

    foreach(int result, results)
    {
        QString item = "0x" + QString("%1").arg(result, 8, 16, QChar('0')).toUpper();
        ui->listOffsets->addItem(item);
    }
}

void MainWindow::on_btnSearchFreeSpace_clicked()
{
    if (_romLoaded)
    {
        int length = 0;
        if (ui->rbtnFreeBytes->isChecked())
        {
            length = ui->sboxFreeBytes->value();
        }
        else
        {
            QString inputText = ui->editInsertText->text();
            inputText = inputText.replace(QChar(' '), "");
            length = inputText.length();
            if (length % 2)
            {
                length = (length / 2) + 1;
            }
            else
            {
                length /= 2;
            }
        }

        ui->listOffsetsSearch->clear();

        QList<int> offsets;
        offsets = rom->readFreeSpaceOffsets(length);
        foreach(int offset, offsets)
        {
            QString item = "0x" + QString("%1").arg(offset, 8, 16, QChar('0')).toUpper();
            ui->listOffsetsSearch->addItem(item);
        }

        statusLabel->setText("Finished - Ready for work");
    }
    else
    {
        statusLabel->setText("No rom loaded");
    }
}

void MainWindow::on_actionLoad_Rom_triggered()
{
    QString path(QFileDialog::getOpenFileName(this, "Select a Rom", QDir::homePath(), "GameBoy Advance ROM (*.gba)"));

    if (_romLoaded)
    {
        delete rom;
        rom = NULL;
    }

    rom = new Rom(path);
    romChanges = new ChangesHandler(rom->romStream());
    _romLoaded = true;
    statusLabel->setText("Ready - Rom loaded");
}

void MainWindow::on_editInsertText_textChanged(const QString &arg1)
{
    ui->editInsertText->setText(arg1.toUpper());
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_btnInsertString_clicked()
{
    if (ui->editInsertText->text().length() >= 2)
    {
        QString input = ui->editInsertText->text();
        input = input.replace(QChar(' '), "");

        QString item;
        int offset;
        if (ui->listOffsetsSearch->currentItem())
        {
            item = ui->listOffsetsSearch->currentItem()->text();
            bool ok;
            offset = item.replace("0x", "").toInt(&ok, 16);
        }
        QByteArray bytes;
        bool ok;
        for (int i = 0; i < input.length(); i += 2)
            bytes.append(input.mid(i, 2).toInt(&ok, 16));

        romChanges->addChange(bytes, offset);
    }
}

void MainWindow::on_actionSave_Rom_triggered()
{
    if (_romLoaded)
    {
        romChanges->writeChanges();
        rom->write(QFileDialog::getSaveFileName(this, "Select a Rom", QDir::homePath(), "GameBoy Advance ROM (*.gba)"));
    }
}

void MainWindow::on_btnRefreshOffsets_clicked()
{
    if (_romLoaded && ui->treeSourceFiles->selectedItems().size())
    {
        QString minimumRaw = ui->treeSourceFiles->selectedItems()[0]->text(1);
        int minimum = minimumRaw.left(minimumRaw.lastIndexOf(" ")).toInt();

        insertOffsets(minimum);
    }
}

void MainWindow::on_actionSelect_Directory_triggered()
{
    setDirectory(QFileDialog::getExistingDirectory(this, "Select a Directory", QDir::homePath()));
}

void MainWindow::on_btnInsertCode_clicked()
{
    if (_romLoaded && ui->listOffsets->count())
    {
        QString fileName = ui->treeSourceFiles->selectedItems()[0]->text(0);
        QFile file(directory().absolutePath() + "/" + fileName + ".bin");
        QByteArray input;

        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "Couldn't read compiled file";
            return;
        }
        input = file.readAll();

        bool conversionOk;
        int offset = ui->listOffsets->itemAt(0, 0)->text().toInt(&conversionOk, 16);

        if (!conversionOk)
        {
            qDebug() << QString::number(offset);
        }
        else
        {
            romChanges->addChange(input, offset);
            romChanges->writeChanges();
        }

        QString minimumRaw = ui->treeSourceFiles->selectedItems()[0]->text(1);
        int minimum = minimumRaw.left(minimumRaw.lastIndexOf(" ")).toInt();

        insertOffsets(minimum);
    }
}

void MainWindow::on_pushButton_clicked()
{
    errorDock->addError("Test", "LOOL");
}
