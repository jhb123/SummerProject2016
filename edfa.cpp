#include "edfa.h"

//QSerialPort *serial;

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

void edfa::EDFAsetPower(float value){
    //maybe make it so value is a qstring
    //This function sets the power by getting the value slider_EDFAPower
    //is set at, and converting it to a Qstring. This is concatenated
    //with the rest of of the command ("APC_number\r\n") and this is
    //converted to a QByteArray. This is a usable form for the write
    //function used by the QSerialPort serial object.


    QString str;
    str.setNum(value);
    QString command = "APC " +str+"\r\n"; //concatinate the string
    QByteArray ba = command.toLatin1(); //convert string to byte array

    serial->write(ba);
    serial->waitForBytesWritten(50);
    serial->readAll();
}
void edfa::EDFAOn(){
    //this function is to take a reading,
    //convert the data from readline to a readable
    //format, and change a label in the main window

    //serial->readAll(); //clears the buffer I think?
    serial->write("OM 2\r\n");
    serial->waitForBytesWritten(50);

    QByteArray buf = serial->readAll();
    QString message(buf);
}

void edfa::EDFAOff(){
    //this function is to take a reading,
    //convert the data from readline to a readable
    //format, and change a label in the main window

    //serial->readAll(); //clears the buffer I think?

    serial->write("OM 0\r\n");
    serial->waitForBytesWritten(50);

    QByteArray buf = serial->readAll();
    QString message(buf);
}

QString edfa::EDFAReadPh1(){
    //this function is to take a reading,
    //convert the data from readline to a readable
    //format, and change a label in the main window

    //serial->readAll(); //clears the buffer I think?
    serial->write("Ph 1\r\n");
    serial->waitForBytesWritten(50);

    QByteArray buf = serial->readAll();
    QString message(buf);
    return message;

}
QString edfa::EDFAReadPh2(){
    //this function is to take a reading,
    //convert the data from readline to a readable
    //format, and change a label in the main window

    serial->write("Ph 2\r\n");
    serial->waitForBytesWritten(50);

    QByteArray buf = serial->readAll();
    QString message(buf);
    return message;
}

