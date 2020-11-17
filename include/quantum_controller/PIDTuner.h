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
public slots:
    void loop();
    void TabChanged(int);
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
    void PitchUpload();

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
    void RollUpload();

private:

    void setupGraph(QCustomPlot*);
    void setupSlots();
    void clearGraph(QCustomPlot*);

    enum DataIndex {state,setPoint,error,correction};
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
    bool  PITCH_GRAPH_DISPLAY[4],ROLL_GRAPH_DISPLAY[4],
    YAW_GRAPH_DISPLAY[4],AUTO_SCROLL_MODE;
    std::string LOGO_PATH,SAVE_PATH,CONFIG_PATH;
    QTimer *timer;
    Ui::PIDTuner *ui;
    int t,BUFFER,DISPLAY_TIME;
    int ACTIVE_WINDOW;

};

#endif // PIDTUNER_H
