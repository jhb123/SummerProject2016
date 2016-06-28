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

private:
    Ui::MainWindow *ui;

    QTimer *wavelengthTimer;
    QTimer *timer;
    bool EDFAOnOff;


    bool statusCheck(RegSetStatusTypes status);
     void checkEmission(); //Function to check emission status. Returns true if emission is on.
     void emissionStateDispUpdate();

     const quint8 devAddress = 10;

     //Laser Variables
     bool emissionOn;
     quint16 wavelength;
     quint16 prevWavelength;


     //Slider
     bool sliderMoving = false;





    IBHandler *interbus;



};

#endif // MAINWINDOW_H
