#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QTimer>
#include <ros/package.h>
#include <ros/ros.h>
#include <stdio.h>
#include <cmath>
#include <string>
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ros::NodeHandle, QWidget *parent = 0);
    ~MainWindow();
public slots:
    void loop();
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


private:

    void setupGraph(QCustomPlot*);
    void setupSlots();
    void clearGraph(QCustomPlot*);
    enum Data {state,setPoint,error,correction};
    bool  PITCH_GRAPH_DISPLAY[4],AUTO_SCROLL_MODE;
    std::string LOGO_PATH,SAVE_PATH,CONFIG_PATH;
    QTimer *timer;
    Ui::MainWindow *ui;
    ros::NodeHandle nh;
    int t,BUFFER,DISPLAY_TIME;

};

#endif // MAINWINDOW_H
