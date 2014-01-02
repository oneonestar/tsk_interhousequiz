#include <questionsPage.h>

QuestionPage::QuestionPage(QWidget * parent, QString id):QWidget(parent){

    Hlayout=new QHBoxLayout();
    Vlayout=new QVBoxLayout();
    next=new QPushButton("Next");
    prev=new QPushButton("Previous");
    A=new QRadioButton("A");
    B=new QRadioButton("B");
    C=new QRadioButton("C");
    D=new QRadioButton("D");
    question=new QLabel("The question");
    correctAns=new QLabel("A");
    this->id=new QLabel(id);
    getAns=new QPushButton("Submit");

    Vlayout->addWidget(this->id);
    Vlayout->addWidget(question);
    Vlayout->addWidget(A);
    Vlayout->addWidget(B);
    Vlayout->addWidget(C);
    Vlayout->addWidget(D);

    Hlayout->addWidget(next);
    Hlayout->addWidget(getAns);
    Hlayout->addWidget(prev);

    Vlayout->addLayout(Hlayout);
   //next->setMinimumHeight(50);
    //prev->setMinimumHeight(50);
    //layout->addWidget(comp);
    //layout->addWidget(chal);
    //next->setGeometry(100,100,100,100);
    //prev->setGeometry(100,100,200,200);
    this->setLayout(Vlayout);

}
