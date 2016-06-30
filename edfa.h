#ifndef EDFA_H
#define EDFA_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QSerialPort>


class edfa: public QObject
{
    Q_OBJECT
public:
    explicit edfa(QObject *parent = 0);
    ~edfa();
    void setUpPort(QString name);
    void EDFASetPower(float value);//set method for power
    void EDFAOn();//set port state
    void EDFAOff();//set port state
    QString EDFAReadPh1();//get method for power
    QString EDFAReadPh2();//get method for power
    bool isReady();//checks if on
private:

    QByteArray buf; //stores data read from the edfa
    QSerialPort *serial; //serial port object
    bool ready;//port ready to be used test
};

#endif // EDFA_H

