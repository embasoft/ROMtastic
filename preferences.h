#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "ui_mainwindow.h"
#include "errordock.h"
#include "filemanager.h"
#include <QFileDialog>
#include <QDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT

public:
    explicit Preferences(FileManager *fileManager, ErrorDock *errorDock, QWidget *parent = 0);
    ~Preferences();

    void createConfigurationFile();
    bool readConfigurationFile();

    // General configuration
    void setCompilerPath(QString path);
    void setWorkingDirectory(QString path);

    // Editor configuration
    void setFont(QString fontName);
    void setFontSize(int size);
    void setFontMonospace(bool value);

    Ui::MainWindow *mainUi() const;
    void setMainUi(Ui::MainWindow *mainUi);

    ErrorDock *errorDock() const;
    void setErrorDock(ErrorDock *errorDock);

    FileManager *fileManager() const;
    void setFileManager(FileManager *fileManager);

    QFile *configurationFile() const;
    void setConfigurationFile(QFile *configurationFile);

    QXmlStreamReader *xmlStreamReader() const;
    void setXmlStreamReader(QXmlStreamReader *xmlStreamReader);

    QXmlStreamWriter *xmlStreamWriter() const;
    void setXmlStreamWriter(QXmlStreamWriter *xmlStreamWriter);

private slots:
    void on_btnSelectWorkingDirectory_clicked();

    void on_btnSelectDevKitARMPath_clicked();

private:
    Ui::Preferences *ui;
    Ui::MainWindow *_mainUi;
    ErrorDock *_errorDock;
    FileManager *_fileManager;
    QFile *_configurationFile;
    QXmlStreamReader *_xmlStreamReader;
    QXmlStreamWriter *_xmlStreamWriter;
};

#endif // PREFERENCES_H
