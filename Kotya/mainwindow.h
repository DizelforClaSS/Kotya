#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ButtonFindFile_clicked();

    void on_radioButton_clicked();

    int chekExi ();

    void on_DeleteWidButton_clicked();

    void on_ButtonPack_clicked();

    void on_SelectName();

    void on_ButtonStore_clicked();

    void on_ButtonUnpack_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_Button_3_clicked();

    void on_Button_4_clicked();

    void on_Button_5_clicked();

    void on_Button_6_clicked();

    void on_ButtonFindFolder_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString fileName;
    QString fileDir;
    int numLab = 0;
};

int test(int argc, char *argv[]);
#endif // MAINWINDOW_H
