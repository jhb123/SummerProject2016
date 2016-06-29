#include "edfa.h"

edfa::edfa(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
}

edfa::~edfa()
{
    serial->close();
}

void edfa::setUpPort(QString name){
    serial->setPortName(name);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);
}

bool edfa::EDFASetPower(float value){
    //EDFA cannot go over 30 dbm!
    if(value < 30){
        QString valueQString;
        valueQString.setNum(value);
        QString command = "APC " + valueQString +"\r\n"; //concatinate the string
        QByteArray ba = command.toLatin1(); //convert string to byte array

        serial->readAll();
        serial->write(ba);
        serial->waitForBytesWritten(50);
        return true;
    }
    else{
        return false;
    }

}
void edfa::EDFAOn(){
    //this function is to take a reading,
    //convert the data from readline to a readable
    //format, and change a label in the main window

    //serial->readAll(); //clears the buffer I think?
    serial->readAll();
    serial->write("OM 2\r\n");
    serial->waitForBytesWritten(50);

}

void edfa::EDFAOff(){
    //this function is to take a reading,
    //convert the data from readline to a readable
    //format, and change a label in the main window

    //serial->readAll(); //clears the buffer I think?
    serial->readAll();
    serial->write("OM 0\r\n");
    serial->waitForBytesWritten(50);
}


//HAHAHA you have to think about recursion
QString edfa::EDFAReadPh1(){
    serial->write("Ph 1\r\n");
    serial->waitForBytesWritten(10000);
    buf = serial->readLine(16); //length of reply is 15 characters
    QString message(buf);
    if (message.contains("PDM1: ",Qt::CaseInsensitive)){
    return message;
    }
    else{
        return EDFAReadPh1();
    }
}
QString edfa::EDFAReadPh2(){
    serial->write("Ph 2\r\n");
    serial->waitForBytesWritten(10000);
    buf = serial->readLine(16);
    QString message(buf);

    if (message.contains("PDM2: ",Qt::CaseInsensitive)){
    return message;
    }
    else{
        return EDFAReadPh2();
    }
}

