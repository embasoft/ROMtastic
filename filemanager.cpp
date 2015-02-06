#include "filemanager.h"

FileManager::FileManager(QWidget *parent) : QTreeWidget(parent)
{

}

FileManager::~FileManager()
{

}

bool FileManager::loadFile(QString path)
{
    QFile file(directory().path() + "/" + path);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QByteArray fileContent = file.readAll();
    editorWidget()->clear();
    editorWidget()->setText(QString(fileContent));
    file.close();

    setFileName(path);
    setFileLoaded(true);

    return true;
}

bool FileManager::saveFile()
{
    QFile file(directory().path() + "/" + fileName());

    if (!file.open(QIODevice::ReadWrite))
        return false;

    if (!file.resize(0))
        return false;

    QString text = editorWidget()->toPlainText();
    QByteArray content;
    content.append(text);

    if (!file.write(content))
        return false;

    file.close();

    return true;
}

int FileManager::compileFile(QString path)
{
    if (_compilerSet)
    {
        compiler()->setDirectory(path.left(path.lastIndexOf("/")));

        QString file;
        file = path.mid(path.lastIndexOf("/") + 1);

        if (!compiler()->compile(file))
        {
            errorDock()->addError("error", "Failed to compile: " + file);
        }
    }
    else
    {
        errorDock()->addError("error", "No compiler set");
    }
}
ErrorDock *FileManager::errorDock() const
{
    return _errorDock;
}

void FileManager::setErrorDock(ErrorDock *errorDock)
{
    _errorDock = errorDock;
}
QTextEdit *FileManager::editorWidget() const
{
    return _editorWidget;
}

void FileManager::setEditorWidget(QTextEdit *editorWidget)
{
    _editorWidget = editorWidget;
}
bool FileManager::fileLoaded() const
{
    return _fileLoaded;
}

void FileManager::setFileLoaded(bool fileLoaded)
{
    _fileLoaded = fileLoaded;
}
QString FileManager::fileName() const
{
    return _fileName;
}

void FileManager::setFileName(const QString &fileName)
{
    _fileName = fileName;
}

bool FileManager::compilerSet()
{
    return _compilerSet;
}

bool FileManager::searchFiles()
{
    if (!_compilerSet)
    {
        errorDock()->addError("error", "Compiler isn't set");
        return false;
    }

    if (directory().path() == "")
    {
        errorDock()->addError("error", "Directory isn't set");
        return false;
    }

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
    addTopLevelItems(files);
}
QDir FileManager::directory() const
{
    return _directory;
}

void FileManager::setDirectory(const QDir &directory)
{
    _directory = directory;
}
Compiler *FileManager::compiler() const
{
    return _compiler;
}

void FileManager::setCompiler(Compiler *compiler)
{
    if (!_compilerSet)
    {
        _compiler = compiler;
        _compilerSet = true;
    }
    else
    {
        delete _compiler;
        _compiler = compiler;
    }
}

void FileManager::setCompilerPath(QString path)
{
    setCompiler(new Compiler(path, this));
}



