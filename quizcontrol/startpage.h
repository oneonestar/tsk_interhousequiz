#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
class Startpage : public QWidget{
    Q_OBJECT

public:
    Startpage(QWidget * parent);
private:
    QPushButton * comp;
    QPushButton * chal;
    QHBoxLayout * layout;


};
#endif // STARTPAGE_H
