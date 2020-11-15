#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QTimer>
#include <ros/package.h>
#include <stdio.h>
#include <cmath>
#include <string>
#include <iostream>
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = 0);
    ~MainWindow();
public slots:
    void loop();
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

private:

    void setupGraph(QCustomPlot*);
    void setupSlots();
    void clearGraph(QCustomPlot*);
    void load();
    float decodePID(uint8_t);
    enum DataIndex {state,setPoint,error,correction};
    enum PIDIndex { pitchP, pitchI, pitchD,
                    rollP,  rollI,  rollD,
                    yawP,   yawI,   yawD };
    uint8_t PID[9];
    bool  PITCH_GRAPH_DISPLAY[4],AUTO_SCROLL_MODE;
    std::string LOGO_PATH,SAVE_PATH,CONFIG_PATH;
    QTimer *timer;
    Ui::MainWindow *ui;
    int t,BUFFER,DISPLAY_TIME;

};

#endif // MAINWINDOW_H
