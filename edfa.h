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
    bool EDFASetPower(float value);
    void EDFAOn();
    void EDFAOff();
    QString EDFAReadPh1();
    QString EDFAReadPh2();

private:

    QByteArray buf;
    QSerialPort *serial;
};

#endif // EDFA_H
