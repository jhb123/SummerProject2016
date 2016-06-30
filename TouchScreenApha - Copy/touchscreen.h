#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <QDialog>
#include "lasersystem.h"
#include <QTimer>

namespace Ui {
class TouchScreen;
}

class TouchScreen : public QDialog
{
    Q_OBJECT

private slots:
    void EDFAEmissionLight(bool emissionOn);
    void laserEmissionLight(bool emissionOn);
    void mainButtonState(bool emissionOn);

    void mainButton();
    void EDFAOnOff();
    void laserOnOff();

public:
    explicit TouchScreen(QWidget *parent = 0);
    ~TouchScreen();


private:
    Ui::TouchScreen *ui;

    //System Object
    LaserSystem *system;

    QTimer *wavelengthTimer;
    QTimer *timer;


     quint16 prevWavelength;


     //Slider
     bool sliderMoving = false;


};

#endif // TOUCHSCREEN_H
