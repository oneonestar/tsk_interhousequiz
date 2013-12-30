#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startpage.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Startpage * s= new Startpage(this);
    QHBoxLayout *layout=new QHBoxLayout();
    //layout->SetMinimumSize(400);
    s->setGeometry(100,100,200,200);
    layout->addWidget(s);
    this->setLayout(layout);

}

MainWindow::~MainWindow()
{
    delete ui;
}
