#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "becreative.h"
#include "errordock.h"
#include <QDir>
#include <QTreeWidget>
#include <QTextEdit>

using namespace beCreative::Utils;

class FileManager : public QTreeWidget
{
public:
    FileManager(QWidget *parent = 0);
    ~FileManager();

    bool loadFile(QString path);
    bool saveFile();
    bool searchFiles();

    QDir directory() const;
    void setDirectory(const QDir &directory);

    Compiler *compiler() const;
    void setCompiler(Compiler *compiler);
    void setCompilerPath(QString path);

    int compileFile(QString Path);

    ErrorDock *errorDock() const;
    void setErrorDock(ErrorDock *errorDock);

    QTextEdit *editorWidget() const;
    void setEditorWidget(QTextEdit *editorWidget);

    bool fileLoaded() const;
    void setFileLoaded(bool fileLoaded);

    QString fileName() const;
    void setFileName(const QString &fileName);

    bool compilerSet();

private:
    bool _fileLoaded = false;
    bool _compilerSet = false;
    QString _fileName;
    QDir _directory;
    Compiler *_compiler;
    ErrorDock *_errorDock;
    QTextEdit *_editorWidget;
};

#endif // FILEMANAGER_H
