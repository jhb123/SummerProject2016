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
    bool setUpPort(QString name);
    void closeComport();

    void EDFAsetPower(float value);

    //Added to make similar to laser - SPG 29/06
    bool setEmission(bool emission);
    bool getEmission();

    bool replyCheck();

    float EDFAReadPh1();
    float EDFAReadPh2();

    bool isReady();

signals:
        void EDFAComportOpen(bool open);

private slots:
        void changeReady(bool set);
        void testSlot();
private:
    QSerialPort *serial;

    bool ready;

    QByteArray buf; //stores data read from the edfa

    void EDFAOn();
    void EDFAOff();

    void delay(int millisecondsToWait);

    QByteArray readSerial();
    void writeSerial(QByteArray serialMsg);

    QString stripString(QString inputString, QString stringIdentifier);

    bool messageFlag;
    bool doneProcess;



};

#endif // EDFA_H
