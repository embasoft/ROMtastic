#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(FileManager *fileManager, ErrorDock *errorDock, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);
    setErrorDock(errorDock);
    setFileManager(fileManager);

    setConfigurationFile(new QFile(QDir::homePath() + "/.romtastic"));
    setXmlStreamReader(new QXmlStreamReader(configurationFile()));
    setXmlStreamWriter(new QXmlStreamWriter(configurationFile()));

    xmlStreamWriter()->setAutoFormatting(true);

    if (!configurationFile()->open(QIODevice::ReadWrite))
    {
        errorDock->addError("error", "Couldn't read configuration");
        return;
    }

    if (!configurationFile()->size())
    {
        createConfigurationFile();
        configurationFile()->seek(0);
    }

    if (!readConfigurationFile())
    {
        errorDock->addError("error", "Error in configuration file");
    }

    configurationFile()->close();
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::createConfigurationFile()
{
    xmlStreamWriter()->writeStartDocument();
    xmlStreamWriter()->writeStartElement("configuration");

    // General configuration
    xmlStreamWriter()->writeStartElement("general");
    xmlStreamWriter()->writeTextElement("workingDirectory", QDir::homePath());
    xmlStreamWriter()->writeTextElement("compilerPath", "");
    xmlStreamWriter()->writeEndElement();

    // Editor configuration
    xmlStreamWriter()->writeStartElement("editor");
    xmlStreamWriter()->writeTextElement("font", "default");
    xmlStreamWriter()->writeTextElement("fontSize", "10");
    xmlStreamWriter()->writeTextElement("fontMonospace", "yes");
    xmlStreamWriter()->writeEndElement();

    xmlStreamWriter()->writeEndElement();
    xmlStreamWriter()->writeEndDocument();
}

bool Preferences::readConfigurationFile()
{
    while (!xmlStreamReader()->atEnd())
    {
        xmlStreamReader()->readNext();
        if (xmlStreamReader()->isStartElement())
        {
            QString identifier = xmlStreamReader()->name().toString();
            if (identifier == "workingDirectory")
            {
                xmlStreamReader()->readNext();
                QString value = xmlStreamReader()->text().toString();
                //setWorkingDirectory(value);
            }
            if (identifier == "compilerPath")
            {
                xmlStreamReader()->readNext();
                QString value = xmlStreamReader()->text().toString();
                setCompilerPath(value);
            }
        }
//        QString tokenString = xmlStreamReader()->tokenString();
//        QString value = xmlStreamReader()->text().toString();

//        if (tokenString == "workingDirectory")
//            setWorkingDirectory(value);
//        else if (tokenString == "compilerPath")
//            setCompilerPath(value);

    }

    if (xmlStreamReader()->hasError())
    {
        errorDock()->addError("error", xmlStreamReader()->errorString());
        return false;
    }

    //qDebug() << fileManager()->directory().path();
    //qDebug() << fileManager()->compiler()->directory();
    return true;
}

void Preferences::setCompilerPath(QString path)
{
    fileManager()->setCompilerPath(path);
}

void Preferences::setWorkingDirectory(QString path)
{
    fileManager()->setDirectory(path);
}

void Preferences::setFont(QString fontName)
{

}

void Preferences::setFontSize(int size)
{

}

void Preferences::setFontMonospace(bool value)
{

}

Ui::MainWindow *Preferences::mainUi() const
{
    return _mainUi;
}

void Preferences::setMainUi(Ui::MainWindow *mainUi)
{
    _mainUi = mainUi;
}
ErrorDock *Preferences::errorDock() const
{
    return _errorDock;
}

void Preferences::setErrorDock(ErrorDock *errorDock)
{
    _errorDock = errorDock;
}

void Preferences::on_btnSelectWorkingDirectory_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select the working directory", QDir::homePath());
    fileManager()->setDirectory(path);
}

void Preferences::on_btnSelectDevKitARMPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select the devkitARM binaries directory", QDir::homePath());
    fileManager()->setCompiler(new Compiler(path, this));
}
QXmlStreamWriter *Preferences::xmlStreamWriter() const
{
    return _xmlStreamWriter;
}

void Preferences::setXmlStreamWriter(QXmlStreamWriter *xmlStreamWriter)
{
    _xmlStreamWriter = xmlStreamWriter;
}

QXmlStreamReader *Preferences::xmlStreamReader() const
{
    return _xmlStreamReader;
}

void Preferences::setXmlStreamReader(QXmlStreamReader *xmlStreamReader)
{
    _xmlStreamReader = xmlStreamReader;
}

QFile *Preferences::configurationFile() const
{
    return _configurationFile;
}

void Preferences::setConfigurationFile(QFile *configurationFile)
{
    _configurationFile = configurationFile;
}

FileManager *Preferences::fileManager() const
{
    return _fileManager;
}

void Preferences::setFileManager(FileManager *fileManager)
{
    _fileManager = fileManager;
}

