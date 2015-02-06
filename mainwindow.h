#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "becreative.h"
#include "errordock.h"
#include "preferences.h"
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

    Preferences *getPreferences() const;
    void setPreferences(Preferences *value);

private:
    bool _romLoaded = false;
    Preferences *preferences;
    QString _romPath;
    QString _compilerPath;

    int compileFile(QString path);
    bool setupSourceBrowser();
    bool setupEditor();
    bool insertOffsets(int minimum);

public slots:

private slots:

    void on_actionLoad_Rom_triggered();

    void on_actionQuit_triggered();

    void on_actionSave_Rom_triggered();

    void on_fileManager_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_actionPreferences_triggered();

    void on_actionSave_File_triggered();

    void on_actionRefresh_Offsets_triggered();

    void on_actionInsert_Code_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
