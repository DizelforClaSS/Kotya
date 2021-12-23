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
    void on_ButtonFindFile_clicked();


       int chekExi ();



       void on_ButtonPack_clicked();


       void on_ButtonStore_clicked();

       void on_ButtonUnpack_clicked();



       void on_Button_3_clicked();

       void on_Button_4_clicked();

       void on_Button_5_clicked();

       void on_Button_6_clicked();

       void on_ButtonFindFolder_clicked();

       void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Archivator* archivator;
    arguments args;
    QString fileName;
        QString fileDir;
        int numLab = 0;

};
#endif // MAINWINDOW_H
