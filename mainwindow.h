#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "becreative.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>

#include "becreative_global.h"

using namespace beCreative::Core;

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
    ~MainWindow();

private:
    bool _romLoaded = false;
    QString _romPath;

private slots:

    void on_pushButton_2_clicked();

    void on_actionLoad_Rom_triggered();

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_actionQuit_triggered();

    void on_pushButton_clicked();

    void on_actionSave_Rom_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
