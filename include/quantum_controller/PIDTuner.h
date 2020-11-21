#ifndef PIDTUNER_H
#define PIDTUNER_H

#include <QMainWindow>
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <ros/package.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include "ui_PIDTuner.h"
#include "quantum_controller/PIDTunerWidget.h"

namespace Ui {
class PIDTuner;
}

class PIDTuner : public QMainWindow
{
    Q_OBJECT

public:
    explicit PIDTuner( QWidget *parent = 0);
    ~PIDTuner();

    void verifyValues(uint16_t [], bool []);
    void updateDataPoints(double [][4], double t);

public slots:
    //Pitch Tuing Window
    // void PitchLoad();
    // void PitchSave();
    // void PitchReset();

    void TabChanged(int);
    //void Upload();

private:

    void initialiseVariables();
    void setupSlots();
    enum DataIndex {state,setPoint,correction,error};
    // enum PIDIndex { pitchP, pitchI, pitchD,
    //                 rollP,  rollI,  rollD,
    //                 yawP,   yawI,   yawD };
    enum TabIndex {pitch, roll, yaw};
    struct pid{
      uint16_t byte=0;
      void setExp(int);
      void setData(int);
      void setValue(float);
      int getData();
      int getExp();
      float getValue();
    };

    pid PID[9];
    int ACTIVE_WINDOW;
    double TIME_OFFSET,T;
    std::string LOGO_PATH;
    Ui::PIDTuner *ui;
    std::vector<PIDTunerWidget*> TABS;


};

#endif // PIDTUNER_H
