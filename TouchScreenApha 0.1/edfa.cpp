/*
 * Todos for this class:
 * -Impliment a more fail safe way of reading data.
 * -Create method to read the replies of commands to ensure they have been read.
 *
 */

#include "edfa.h"
#include<QDebug>
#include<QTimer>
#include <QThread>
#include<QTime>
#include<QCoreApplication>

edfa::edfa(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    connect(serial, SIGNAL(dataTerminalReadyChanged(bool)), this, SLOT(changeReady(bool))); //Updates the ready state if the edfa is unplugged.
    ready = false;
}

edfa::~edfa()
{
    serial->close();
}
 //Changed this a bit to make it more error proof. SPG 29/06

bool edfa::setUpPort(QString name){
    serial -> close();
    serial->setPortName(name);
    if(serial->open(QIODevice::ReadWrite)){

    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);    
    ready = true;

    emit EDFAComportOpen(serial-> isOpen());
    return true;
    }
    return false;
}

void edfa::closeComport(){
    serial -> close();
    ready = false;


    emit EDFAComportOpen(serial -> isOpen());
}


void edfa::EDFAsetPower(float value){
    //EDFA cannot go over 30 dbm!
    if (ready){

        if(value < 30){
            QString valueQString;
            valueQString.setNum(value);
            QString command = "APC " + valueQString +"\r\n"; //concatinate the string
            QByteArray ba = command.toLatin1(); //convert string to byte array

            serial->readAll();
            serial->write(ba);
            serial->waitForBytesWritten(50);
        }
    }
}

bool edfa::replyCheck(){
    qDebug() << "In reply check";
    if(ready){
            qDebug() << "In reply check ready pass";
           serial->clear();
           serial->write("giberish\r\n"); //send some giberish to get FWC response.
           serial->waitForBytesWritten(10000);
           QString message = readSerial();
           qDebug() << message;
           if (message.contains("FWC",Qt::CaseInsensitive)){
           return true;
           }
           else{
               return replyCheck();
           }

    }
    return false;
}

bool edfa::setEmission(bool emission){
            qDebug() << "Set emission in EDFA object " << emission;
    if(emission){

        EDFAOn();
    }
    else{
        EDFAOff();
    }

    qDebug() << readSerial(); //Needed for some reason. Think it is to do with delays.

}

void edfa::EDFAOn(){
    //this function is to take a reading,
    //convert the data from readline to a readable
    //format, and change a label in the main window
    if(ready){
        qDebug() << "Final EDFA write on";
        serial->readAll();
        serial->write("OM 2\r\n");
        serial->waitForBytesWritten(50);
    }
}

void edfa::EDFAOff(){
    //this function is to take a reading,
    //convert the data from readline to a readable
    //format, and change a label in the main window

    //serial->readAll(); //clears the buffer I think?
    if(ready){
        qDebug() << "Final EDFA write off";
        serial->readAll();
        serial->write("OM 0\r\n");
        serial->waitForBytesWritten(50);
    }
}

bool edfa::getEmission(){
    bool emission = false; //This needs to be made better so that the method will return something if no emission is found.
    QString message;

    if(ready){
        writeSerial("OM\r\n");
        message = readSerial();
        qDebug() << "get EDFA emission message " << message;
        if (message.contains("OM ",Qt::CaseInsensitive)){
            message = stripString(message, "OM");
            qDebug() << "message contains OM";
            qDebug() << "New message " << message;
        }
    }
    if(message == "O"){
        emission = false;
    }
    if(message == "2"){
        emission = true;
    }

return emission;
}

float edfa::EDFAReadPh1(){
    QString message;

    if(ready){
        writeSerial("Ph 1\r\n");
        message=readSerial();
        if (message.contains("PDM1: ",Qt::CaseInsensitive)){
            message = stripString(message, "PDM1:");
        }
    }

    return message.toFloat();
}


float edfa::EDFAReadPh2(){
    QString message;

    if(ready){
        writeSerial("Ph 2\r\n");
        message=readSerial();
        if (message.contains("PDM2: ",Qt::CaseInsensitive)){
            message = stripString(message, "PDM2:");
        }
    }

    return message.toFloat();
}

bool edfa::isReady(){
    return ready;
}

void edfa::changeReady(bool set){
    ready = set;
    qDebug() << "EDFA usb state changed to " << ready;
}

void edfa::testSlot(){
    while(!serial->atEnd()){
        buf = serial->read(100);
    }
    QString message = buf;
    qDebug() << "get EDFA emission message " << message;
    if (message.contains("OM ",Qt::CaseInsensitive)){
        messageFlag = true;
        qDebug() << "message contains OM";

    }
    else{
        getEmission();
    }

}

void edfa::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

QByteArray edfa::readSerial(){
   QByteArray buf;
    delay(200);
    while(!serial->atEnd()){
        buf = serial->read(100);
    }

    return buf;
}

void edfa::writeSerial(QByteArray serialMsg){
    serial->clear();
    serial->write(serialMsg);
    serial->waitForBytesWritten(10000);
}

//Function to strip the EDFA string of the carridge return and starting value.
QString edfa::stripString(QString inputString, QString stringIdentifier){
    inputString = inputString.replace("\r", "");
    inputString = inputString.replace("\n", "");
    inputString = inputString.replace(stringIdentifier, "");
    inputString = inputString.replace(" ", "");
    serial->clear();

    return inputString;

}
