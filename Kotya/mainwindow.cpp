#include "mainwindow.h"
#include "iostream"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QFileDialog>
#include <QRadioButton>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ButtonFindFile_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,
                            QString::fromUtf8("Открыть файл"),
                            QDir::currentPath());
    ui->label_file->setText(fileName);
}

void MainWindow::on_ButtonPack_clicked()
{
    if (chekExi() != 1)
    {
        //здесь вызов архивации
    }

}


void MainWindow::on_ButtonStore_clicked()
{
    if (chekExi() != 1)
    {
        //здесь вызов архивации
    }
}

void MainWindow::on_ButtonUnpack_clicked()
{
    if (chekExi() != 1)
    {
        //здесь вызов архивации
    }
}

int MainWindow::chekExi ()
{
    int f = 0;
    if (ui->label_file->text() == "")
    {
        ui->info->setText("не задан файл");
        f = 1;
    }
    if (ui->label_folder->text() == "")
    {
        ui->info->setText("не задана папка");
        f = 1;
    }
    return f;
}

void MainWindow::on_Button_3_clicked()
{
    numLab = 3;
}


void MainWindow::on_Button_4_clicked()
{
    numLab = 4;
}


void MainWindow::on_Button_5_clicked()
{
    numLab = 5;
}


void MainWindow::on_Button_6_clicked()
{
    numLab = 6;
}





void MainWindow::on_pushButton_clicked()
{
    ui->info->setText("не задана папка");
}


void MainWindow::on_ButtonFindFolder_clicked()
{
    fileDir = QFileDialog::getExistingDirectory(this,
                                                 QString::fromUtf8("Открыть папку"),
                                                 QDir::currentPath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->label_folder->setText(fileDir);
}

