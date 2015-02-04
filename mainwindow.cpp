#include "becreative.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusLabel = new QLabel("Ready - No rom loaded...");
    ui->statusBar->addWidget(statusLabel);

    errorDock = new ErrorDock(ui->trErrors);

    if (!setupSourceBrowser())
    {
        errorDock->addError("error", "Setup of file manager failed");
    }
    if (!setupEditor())
    {
        errorDock->addError("error", "Setup of editor failed");
    }

    QFont font = ui->editSource->font();
    font.setStyleHint(QFont::Monospace);
    ui->editSource->setFont(font);
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
    ui->treeSourceFiles->setCompiler(new Compiler(compilerPath, this));
    ui->treeSourceFiles->searchFiles();
}

QDir MainWindow::directory() const
{
    return _directory;
}

void MainWindow::setDirectory(const QDir &directory)
{
    _directory = directory;
    ui->treeSourceFiles->setDirectory(directory);

    if(ui->treeSourceFiles->compilerSet())
        ui->treeSourceFiles->searchFiles();
}

bool MainWindow::setupEditor()
{
    highlighter = new Highlighter(ui->editSource->document());

    return true;
}

bool MainWindow::setupSourceBrowser()
{
    ui->treeSourceFiles->setDirectory(QDir::homePath());
    ui->treeSourceFiles->setErrorDock(errorDock);
    ui->treeSourceFiles->setEditorWidget(ui->editSource);

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

    return true;
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

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionSave_Rom_triggered()
{
    if (_romLoaded)
    {
        romChanges->writeChanges();
        rom->write(QFileDialog::getSaveFileName(this, "Select a Rom", QDir::homePath(), "GameBoy Advance ROM (*.gba)"));
    }
    else
    {
        errorDock->addError("hint", "No rom loaded");
    }
}

void MainWindow::on_btnRefreshOffsets_clicked()
{
    if (_romLoaded)
    {
        if (ui->treeSourceFiles->selectedItems().size())
        {
            QString minimumRaw = ui->treeSourceFiles->selectedItems()[0]->text(1);
            int minimum = minimumRaw.left(minimumRaw.lastIndexOf(" ")).toInt();

            insertOffsets(minimum);
        }
        else
        {
            errorDock->addError("hint", "No file selected");
        }
    }
    else
    {
        errorDock->addError("hint", "No rom loaded");
    }
}

void MainWindow::on_actionSelect_Directory_triggered()
{
    setDirectory(QFileDialog::getExistingDirectory(this, "Select a Directory", QDir::homePath()));
}

void MainWindow::on_btnInsertCode_clicked()
{
    if (_romLoaded)
    {
        if (ui->listOffsets->count())
        {
            QString fileName = ui->treeSourceFiles->selectedItems()[0]->text(0);
            QFile file(directory().absolutePath() + "/" + fileName + ".bin");
            QByteArray input;

            if (!file.open(QIODevice::ReadOnly))
            {
                errorDock->addError("error", "Couldn't read compiled file");
                return;
            }
            input = file.readAll();

            bool conversionOk;
            int offset = ui->listOffsets->itemAt(0, 0)->text().toInt(&conversionOk, 16);

            if (!conversionOk)
            {
                errorDock->addError("error", "Couldn't convert offset string to integer");
            }
            else
            {
                romChanges->addChange(input, offset);
                romChanges->writeChanges();
            }

            errorDock->addError("hint", "Code inserted at: " + ui->listOffsets->itemAt(0, 0)->text());

            QString minimumRaw = ui->treeSourceFiles->selectedItems()[0]->text(1);
            int minimum = minimumRaw.left(minimumRaw.lastIndexOf(" ")).toInt();

            insertOffsets(minimum);
        }
        else
        {
            errorDock->addError("error", "No free space available");
        }
    }
    else
    {
        errorDock->addError("hint", "No rom loaded");
    }
}

void MainWindow::on_btnSaveFile_clicked()
{
    if (ui->treeSourceFiles->fileLoaded())
    {
        if (!ui->treeSourceFiles->saveFile())
        {
            errorDock->addError("error", "Failed to save file: " + ui->treeSourceFiles->fileName());
        }
    }
    else
    {
        errorDock->addError("hint", "No file loaded");
    }
}

void MainWindow::on_treeSourceFiles_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString fileName = item->text(0);

    if (!ui->treeSourceFiles->loadFile(fileName))
    {
        errorDock->addError("error", "Failed to load file: " + fileName);
        return;
    }

    ui->boxRom->setTitle("Rom - " + fileName);
}

void MainWindow::on_actionSet_compiler_path_triggered()
{
    setCompilerPath(QFileDialog::getExistingDirectory(this, "Select the directory of the compiler", QDir::homePath()));
}
