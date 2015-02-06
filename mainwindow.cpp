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

    preferences = new Preferences(ui->fileManager, errorDock, this);
    preferences->setMainUi(ui);
}

MainWindow::~MainWindow()
{
    delete ui;
}
Preferences *MainWindow::getPreferences() const
{
    return preferences;
}

void MainWindow::setPreferences(Preferences *value)
{
    preferences = value;
}


bool MainWindow::setupEditor()
{
    ui->editSource->setHighlighter(new Highlighter(ui->editSource->document()));

    return true;
}

bool MainWindow::setupSourceBrowser()
{
    ui->fileManager->setDirectory(QDir::homePath());
    ui->fileManager->setErrorDock(errorDock);
    ui->fileManager->setEditorWidget(ui->editSource);

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

void MainWindow::on_fileManager_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString fileName = item->text(0);

    if (!ui->fileManager->loadFile(fileName))
    {
        errorDock->addError("error", "Failed to load file: " + fileName);
        return;
    }

    ui->boxRom->setTitle("Rom - " + fileName);
}

void MainWindow::on_actionPreferences_triggered()
{
    preferences->show();
}

void MainWindow::on_actionSave_File_triggered()
{
    if (ui->fileManager->fileLoaded())
    {
        if (!ui->fileManager->saveFile())
        {
            errorDock->addError("error", "Failed to save file: " + ui->fileManager->fileName());
        }
    }
    else
    {
        errorDock->addError("hint", "No file loaded");
    }
}

void MainWindow::on_actionRefresh_Offsets_triggered()
{
    if (_romLoaded)
    {
        if (ui->fileManager->selectedItems().size())
        {
            QString minimumRaw = ui->fileManager->selectedItems()[0]->text(1);
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

void MainWindow::on_actionInsert_Code_triggered()
{
    if (_romLoaded)
    {
        if (ui->listOffsets->count())
        {
            QString fileName = ui->fileManager->selectedItems()[0]->text(0);
            QFile file(ui->fileManager->directory().absolutePath() + "/" + fileName + ".bin");
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

            QString minimumRaw = ui->fileManager->selectedItems()[0]->text(1);
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
