#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "archivator.h"
#include "Structures.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void parse();

private:
    Ui::MainWindow *ui;
    Archivator* archivator;
    arguments args;

};
#endif // MAINWINDOW_H
