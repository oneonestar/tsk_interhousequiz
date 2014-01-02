#ifndef QUESTIONSPAGE_H
#define QUESTIONSPAGE_H

#include <QtGui>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>

class QuestionPage : public QWidget{

    Q_OBJECT

public:
    QuestionPage(QWidget * parent, QString id);
private:
    QPushButton * next;
    QPushButton * prev;
    QHBoxLayout * Hlayout;
    QVBoxLayout * Vlayout;

    QRadioButton * A;
    QRadioButton * B;
    QRadioButton * C;
    QRadioButton * D;
    QPushButton * getAns;
    QLabel * id;

   QLabel * question;
   QLabel * correctAns;

};

#endif // QUESTIONSPAGE_H
