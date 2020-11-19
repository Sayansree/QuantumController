#ifndef PIDTUNER_H
#define PIDTUNER_H

#include <QMainWindow>
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <ros/package.h>
#include <stdio.h>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include "ui_PIDTuner.h"

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
    void updateDataPoints(double [][3], double t);

public slots:
    //Pitch Tuing Window
    void PitchGraph(int);
    void PitchGraphClear();
    void PitchGraphSave();
    void PitchGraphAutoScroll();
    void PitchGraphInspect(bool);
    void PitchErrorGraph(int);
    void PitchSetpointGraph(int);
    void PitchCorrectionGraph(int);
    void PitchBufferChanged(int);
    void PitchDisplayChanged(int);
    void PitchPChanged(int);
    void PitchIChanged(int);
    void PitchDChanged(int);
    void PitchPFPChanged(int);
    void PitchIFPChanged(int);
    void PitchDFPChanged(int);
    void PitchLoad();
    void PitchSave();
    void PitchReset();

    //Roll Tuning Window
    void RollGraph(int);
    void RollGraphClear();
    void RollGraphSave();
    void RollGraphAutoScroll();
    void RollGraphInspect(bool);
    void RollErrorGraph(int);
    void RollSetpointGraph(int);
    void RollCorrectionGraph(int);
    void RollBufferChanged(int);
    void RollDisplayChanged(int);
    void RollPChanged(int);
    void RollIChanged(int);
    void RollDChanged(int);
    void RollPFPChanged(int);
    void RollIFPChanged(int);
    void RollDFPChanged(int);
    void RollLoad();
    void RollSave();
    void RollReset();

    //Yaw Tuning Window
    void YawGraph(int);
    void YawGraphClear();
    void YawGraphSave();
    void YawGraphAutoScroll();
    void YawGraphInspect(bool);
    void YawErrorGraph(int);
    void YawSetpointGraph(int);
    void YawCorrectionGraph(int);
    void YawBufferChanged(int);
    void YawDisplayChanged(int);
    void YawPChanged(int);
    void YawIChanged(int);
    void YawDChanged(int);
    void YawPFPChanged(int);
    void YawIFPChanged(int);
    void YawDFPChanged(int);
    void YawLoad();
    void YawSave();
    void YawReset();

    void TabChanged(int);
    void Upload();

private:

    void initialiseVariables();
    void setupGraph(QCustomPlot*,QString);
    void setupSlots();
    void clearGraph(QCustomPlot*);

    enum DataIndex {state,setPoint,correction,error};
    enum PIDIndex { pitchP, pitchI, pitchD,
                    rollP,  rollI,  rollD,
                    yawP,   yawI,   yawD };
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
    double TIME_OFFSET,DISPLAY_TIME;
    int ACTIVE_WINDOW,BUFFER[3];
    bool  PITCH_GRAPH_DISPLAY[4],ROLL_GRAPH_DISPLAY[4],
    YAW_GRAPH_DISPLAY[4],*GRAPH_DISPLAY,AUTO_SCROLL_MODE;
    std::string LOGO_PATH,SAVE_PATH,CONFIG_PATH;
    Ui::PIDTuner *ui;
    QCustomPlot *ACTIVE_GRAPH;


};

#endif // PIDTUNER_H
