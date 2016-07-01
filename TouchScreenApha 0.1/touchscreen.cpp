#include "touchscreen.h"
#include "ui_touchscreen.h"

#include <QTimer>
#include <QDebug>
#include <QDebug>

TouchScreen::TouchScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TouchScreen)
{
    mainSystem = new LaserSystem(this);

    ui->setupUi(this);

    connect(mainSystem, SIGNAL(EDFAEmission(bool)), this, SLOT(EDFAEmissionLight(bool)));
    connect(mainSystem, SIGNAL(laserEmission(bool)), this, SLOT(laserEmissionLight(bool)));
    connect(mainSystem, SIGNAL(systemEmission(bool)), this, SLOT(mainButtonState(bool)));

    connect(ui->EDFAOn, SIGNAL(pressed()), this, SLOT(EDFAOnOff()));
    connect(ui->EDFAOff, SIGNAL(pressed()), this, SLOT(EDFAOnOff()));

    connect(ui->TurnOnBtn, SIGNAL(pressed()), this, SLOT(mainButton()));
    connect(ui->TurnOffBtn, SIGNAL(pressed()), this, SLOT(mainButton()));

    connect(ui->LaserQuestOff, SIGNAL(pressed()), this, SLOT(laserOnOff()));
    connect(ui->LaserQuestOn, SIGNAL(pressed()), this, SLOT(laserOnOff()));

    mainSystem -> updateSystemState();

    ui->StableOff->show();
    ui->StableOn->hide();

    //Change these timers so that all of the system variables are checked at intervals
    QTimer *wavelengthTimer = new QTimer(this);
    connect(wavelengthTimer, SIGNAL(timeout()),this, SLOT(wavelength = ui->hsWavelength->value()));

    //Setup similar system to origional interface where the button type is automatically updated uing signals.


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(checkLaser()));
    timer->start(250);
}

TouchScreen::~TouchScreen()
{
    delete ui;
}



void TouchScreen::EDFAEmissionLight(bool emissionOn){
    qDebug() << "Updated EDFA button";
    if(emissionOn){
        ui->EDFAOff->hide();
        ui->EDFAOn->show();
    }
    else{
        ui->EDFAOff->show();
        ui->EDFAOn->hide();
    }
}

void TouchScreen::laserEmissionLight(bool emissionOn){
    qDebug() << "Updated laser button";
    if(emissionOn){
        ui->LaserQuestOff->hide();
        ui->LaserQuestOn->show();
    }
    else{
        ui->LaserQuestOff->show();
        ui->LaserQuestOn->hide();
    }
}

void TouchScreen::mainButtonState(bool emissionOn){
    if(emissionOn){
        ui->TurnOnBtn->show();
        ui->TurnOffBtn-> hide();
    }else{
        ui->TurnOnBtn->hide();
        ui->TurnOffBtn->show();
    }

}

void TouchScreen::mainButton(){
        qDebug() << "Main button press";
    mainSystem -> setEmission(ui->TurnOnBtn->isHidden());

}

void TouchScreen::EDFAOnOff(){
        qDebug() << "EDFA Button";
    mainSystem -> setEDFAEmission(ui->EDFAOn->isHidden());

}

void TouchScreen::laserOnOff(){
    qDebug() << "Laser Button";
    bool hidden = ui->LaserQuestOn->isHidden();
    qDebug() << "Red on button is hidden " << hidden;
    mainSystem -> setLaserEmission(hidden);

}


