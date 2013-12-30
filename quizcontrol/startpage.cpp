#include "startpage.h"

Startpage::Startpage(QWidget * parent):QWidget(parent){
    layout=new QHBoxLayout();
    comp=new QPushButton("Compulsory");
    chal=new QPushButton("Challenge");
    comp->setMinimumHeight(50);
    chal->setMinimumHeight(50);
    layout->addWidget(comp);
    layout->addWidget(chal);
    comp->setGeometry(100,100,100,100);
    chal->setGeometry(100,100,200,200);
    this->setLayout(layout);
}
