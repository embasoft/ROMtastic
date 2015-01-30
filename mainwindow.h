#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "becreative.h"
#include "errordock.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>

#include "becreative_global.h"

using namespace beCreative::Core;
using namespace beCreative::Utils;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Rom *rom;
    ChangesHandler *romChanges;
    QLabel *statusLabel;
    Compiler *compiler;
    ErrorDock *errorDock;
    ~MainWindow();

    QString compilerPath() const;
    void setCompilerPath(const QString &compilerPath);

    QDir directory() const;
    void setDirectory(const QDir &directory);

private:
    bool _romLoaded = false;
    QString _romPath;
    QString _compilerPath;
    QDir _directory;
    int compileFile(QString path);
    bool initializeSourceBrowser();
    bool insertOffsets(int minimum);

public slots:

private slots:

    void on_btnSearchFreeSpace_clicked();

    void on_actionLoad_Rom_triggered();

    void on_editInsertText_textChanged(const QString &arg1);

    void on_actionQuit_triggered();

    void on_btnInsertString_clicked();

    void on_actionSave_Rom_triggered();

    void on_btnRefreshOffsets_clicked();

    void on_actionSelect_Directory_triggered();

    void on_btnInsertCode_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
