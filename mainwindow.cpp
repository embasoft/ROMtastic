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
    ui->lineEdit_2->setValidator(regExpValidator);
    statusLabel = new QLabel("Ready - No rom loaded...");
    ui->statusBar->addWidget(statusLabel);

    QDir home(QDir::homePath() + "/romhacking");
    home.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QList<QTreeWidgetItem *> files;

    QStringList filters;
    filters << "*.asm";
    home.setNameFilters(filters);

    foreach(QString file, home.entryList())
    {
        files.append(new QTreeWidgetItem(QStringList(file), 0));
    }
    ui->treeWidget->addTopLevelItems(files);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    if (_romLoaded)
    {
        int length = 0;
        if (ui->radioButton->isChecked())
        {
            length = ui->spinBox->value();
        }
        else
        {
            QString inputText = ui->lineEdit_2->text();
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

        ui->listWidget->clear();

        QList<int> offsets;
        offsets = rom->readFreeSpaceOffsets(length);
        foreach(int offset, offsets)
        {
            QString item = "0x" + QString("%1").arg(offset, 8, 16, QChar('0')).toUpper();
            ui->listWidget->addItem(item);
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

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    ui->lineEdit_2->setText(arg1.toUpper());
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->lineEdit_2->text().length() >= 2)
    {
        QString input = ui->lineEdit_2->text();
        input = input.replace(QChar(' '), "");

        QString item;
        int offset;
        if (ui->listWidget->currentItem())
        {
            item = ui->listWidget->currentItem()->text();
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
    romChanges->writeChanges();
    rom->write(QFileDialog::getSaveFileName(this, "Select a Rom", QDir::homePath(), "GameBoy Advance ROM (*.gba)"));
}
