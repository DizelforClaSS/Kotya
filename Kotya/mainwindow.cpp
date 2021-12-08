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

int test(int argc, char *argv[])
{

}

void MainWindow::on_ButtonFindFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                            QString::fromUtf8("Открыть файл"),
                            QDir::currentPath());

    QRadioButton *selectedFile = new QRadioButton(fileName, this);
    connect(selectedFile, &QPushButton::clicked, this, &MainWindow::on_radioButton_clicked);
    ui->verticalLayout->addWidget(selectedFile);
}

void MainWindow::on_radioButton_clicked()
{
    QRadioButton *selectedSender = qobject_cast<QRadioButton*>(sender());
    if(selectedSender->isChecked())
    {
        fileName = selectedSender;
        cout << fileName->text().toStdString() << endl;
        ui->label->setText(fileName->text());
     }
    else
    {
        ui->label->clear();
    }

}

void MainWindow::on_ButtonPack_clicked()
{
    std::string str = fileName->text().toStdString();
}

