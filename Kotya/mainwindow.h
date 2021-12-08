#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>

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

    void on_DeleteWidButton_clicked();

    void on_ButtonPack_clicked();

    void on_SelectName();

private:
    Ui::MainWindow *ui;
    QRadioButton* fileName;
};

int test(int argc, char *argv[]);
#endif // MAINWINDOW_H
