#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QFileDialog>
#include <QRadioButton>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    archivator = new Archivator();
    connect(ui->folderButton, SIGNAL(clicked()), this, SLOT(folderButtonClicked()));
   // parse();

}

MainWindow::~MainWindow()
{
    delete archivator;
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
        args.archivePath = ui->label_folder->text().toStdString();// "E:\\encode_files.kotik"; //pack - директорию unpack - указываемым сам файл распакови
        args.directoryPath = ui->label_file->text().toStdString(); //наоборот
        args.compression = compression;
        args.labAddition = numLab;
        archivator->pack(args);
    }

}


void MainWindow::on_ButtonStore_clicked()
{
    if (chekExi() != 1)
    {
        args.archivePath = ui->label_file->text().toStdString();
        args.directoryPath = ui->label_folder->text().toStdString();
        archivator->printFiles(args);
    }
}

void MainWindow::on_ButtonUnpack_clicked()
{
    if (chekExi() != 1)
    {
        cout<<"Start"<<endl;
        args.archivePath = ui->label_file->text().toStdString();// "E:\\encode_files.kotik"; //pack - директорию unpack - указываемым сам файл распакови
        args.directoryPath = ui->label_folder->text().toStdString()+"\\"; //наоборот
        args.compression = compression;
        args.labAddition = numLab;
        archivator->unpack(args);
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
    compression = 0;

}


void MainWindow::on_Button_4_clicked()
{
    numLab = 4;
    compression = 1;
}


void MainWindow::on_Button_5_clicked()
{
    numLab = 5;
    compression = 3;
}


void MainWindow::on_Button_6_clicked()
{
    numLab = 6;
    compression = 5;
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

void MainWindow::folderButtonClicked()
{
    fileDir = QFileDialog::getExistingDirectory(this,
                                                 QString::fromUtf8("Открыть папку"),
                                                 QDir::currentPath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->label_file->setText(fileDir);
}


void MainWindow::parse()
{
  cout<<"Start"<<endl;
  args.archivePath = "E:\\A_TEST"; //pack - директорию unpack - указываемым сам файл распакови
  args.directoryPath = "C:\\Kotya\\TestFiles\\test.txt"; //наоборот
  args.compression = 6;
  args.labAddition = 0;
  args.type = 2;
  archivator->pack(args);
}


void MainWindow::on_Button_7_clicked()
{
    numLab = 7;
    compression = 6;
}

