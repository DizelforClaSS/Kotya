#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    archivator = new Archivator();
    parse();
}

MainWindow::~MainWindow()
{
    delete archivator;
    delete ui;
}

void MainWindow::parse()
{
  cout<<"Start"<<endl;
  args.archivePath = "E:\\encode_files.kotik"; //pack - директорию unpack - указываемым сам файл распакови
  args.directoryPath = "E:\\decode"; //наоборот
  args.compression = 6;
  args.labAddition = 0;
  args.type = 2;
  archivator->unpack(args);
}

