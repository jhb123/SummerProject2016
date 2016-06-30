
#include "QtWidgets"

#include "Testbut.h"

TestBut::TestBut(QWidget *parent) :
    QWidget(parent)
{
    buttonState = false;
    display = new QLineEdit("Test");
    display -> setReadOnly(true);
    display-> setAlignment(Qt::AlignRight);
    display -> setMaxLength(15);

    QFont font = display->font();
    font.setPointSize(font.pointSize()+8);
    display->setFont(font);

    updateText();

    //QToolButton *onOffButton = createButton(tr("On/Off"), SLOT(buttonPress()));

    onOffButton = new QPushButton("On/Off", this);
    connect(onOffButton, SIGNAL(pressed()), this, SLOT(buttonPress()));
    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(display, 0, 0, 1, 6);
    mainLayout->addWidget(onOffButton, 1,0,1,2);

    setLayout(mainLayout);

    setWindowTitle(tr("Test"));
}
void TestBut::updateText(){
    if(buttonState==true){
       display -> setText(tr("On"));

    }
    else{
        display -> setText(tr("Off"));
    }
}

void TestBut::buttonPress(){
    buttonState = !buttonState;
    updateText();

}
