#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     stackedlayout=new QStackedLayout(this);
    s= new Startpage(this);
    this->index=0;
   q=new QuestionPage(this,"Question 1");
    this->index++;
    QHBoxLayout *layout=new QHBoxLayout();
    //layout->SetMinimumSize(400);
    s->setGeometry(0,0,200,200);
    q->setGeometry(0,0,200,200);
    q->setVisible(false);
    stackedlayout->addWidget(s);
    stackedlayout->addWidget(q);

    connect(s->comp, SIGNAL(clicked()),
            this, SLOT(changePage()));

    this->setLayout(layout);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changePage(){
    s->setVisible(false);
    q->setVisible(true);

}
