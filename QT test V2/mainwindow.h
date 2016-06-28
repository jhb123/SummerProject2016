#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Interbus/ibhandler.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void butOpenClose();
    void switchEmissionState();
    void checkWavelength();
    void updateLaserState();
    void setWavelength();
    void EDFAOnOffSwitch();




    // IBHandler slot
    void comportOpened(QString portname, bool opened);
public slots:
    void myfunction();

private:
    Ui::MainWindow *ui;

    QSerialPort *serialEDFA;

    QTimer *wavelengthTimer;
    QTimer *timer;
    bool EDFAOnOff;



     void checkEmission(); //Function to check emission status. Returns true if emission is on.
     void emissionStateDispUpdate();

     void setupComports();

     bool regStatusCheck(RegGetStatusTypes regStatus); //Overloaded status check method
     bool regStatusCheck(RegSetStatusTypes regStatus);

     const quint8 devAddress = 10; //Adress of laser.

     //Laser Variables
     bool emissionOn;
     quint16 wavelength;
     quint16 prevWavelength;


     //Slider
     bool sliderMoving = false;

     QString laserComport; //Stores the name of the comport the laser is connected to.





    IBHandler *interbus;



};

#endif // MAINWINDOW_H
