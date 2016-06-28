#ifndef TESTBUT_H
#define TESTBUT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE
class QPushButton;

class TestBut : public QWidget
{
    Q_OBJECT

public:
    explicit TestBut(QWidget *parent = 0);


private slots:
    void buttonPress();


private:
    //QToolButton *createButton(const QString &text, const char *member);
    QPushButton *onOffButton;
    QLineEdit *display;

    void updateText();

    bool buttonState;



};

#endif // TESTBUT_H
