#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "becreative.h"
#include "errordock.h"
#include "highlighter.h"
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

    Highlighter *highlighter;

    int compileFile(QString path);
    bool setupSourceBrowser();
    bool setupEditor();
    bool insertOffsets(int minimum);

public slots:

private slots:

    void on_actionLoad_Rom_triggered();

    void on_actionQuit_triggered();

    void on_actionSave_Rom_triggered();

    void on_btnRefreshOffsets_clicked();

    void on_actionSelect_Directory_triggered();

    void on_btnInsertCode_clicked();

    void on_btnSaveFile_clicked();

    void on_treeSourceFiles_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_actionSet_compiler_path_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
