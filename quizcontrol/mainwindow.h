#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QStackedLayout>
#include "startpage.h"
#include "questionsPage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QStackedLayout * stackedlayout;
    Startpage * s;
    QuestionPage * q;

private:
    Ui::MainWindow *ui;
    int index;
public slots:
    void changePage();



};

#endif // MAINWINDOW_H
