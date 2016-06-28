#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <QDialog>
#include "Interbus/ibhandler.h"
#include <QTimer>

namespace Ui {
class TouchScreen;
}

class TouchScreen : public QDialog
{
    Q_OBJECT

private slots:

    void butOpenClose();
    void checkWavelength();
    void updateLaserState();
    void setSystemState();



    // IBHandler slot
    void comportOpened(QString portname, bool opened);

public:
    explicit TouchScreen(QWidget *parent = 0);
    ~TouchScreen();


private:
    Ui::TouchScreen *ui;

    QSerialPort *serialEDFA;

    QTimer *wavelengthTimer;
    QTimer *timer;
    bool EDFAOnOff;


    bool statusCheck(RegSetStatusTypes status);
     void checkEmission(); //Function to check emission status. Returns true if emission is on.

     const quint8 devAddress = 10;

     void setupComports();

     bool regStatusCheck(RegGetStatusTypes regStatus); //Overloaded status check method
     bool regStatusCheck(RegSetStatusTypes regStatus);

     QString laserComport; //Stores the name of the comport the laser is connected to.

     //Laser Variables
     bool emissionOn;
     quint16 wavelength;
     quint16 prevWavelength;


     //Slider
     bool sliderMoving = false;





    IBHandler *interbus;

};

#endif // TOUCHSCREEN_H
