#ifndef PIDTUNERWIDGET_H
#define PIDTUNERWIDGET_H

#include <QWidget>
#include <QDebug>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <ros/package.h>
#include "ui_PIDTunerWidget.h"

namespace Ui {
class PIDTunerWidget;
}

class PIDTunerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PIDTunerWidget(QString, QWidget*);
    ~PIDTunerWidget();

    void updateDataPoints(double [4], double t);//
    void tabKill();
    //void veri
public slots:
    // Tuing Window
    void GraphClear();//
    void GraphSave();//
    void GraphAutoScroll();//
    void GraphInspect(bool);//
    void ValueGraph(int);//
    void ErrorGraph(int);//
    void SetpointGraph(int);//
    void CorrectionGraph(int);//
    void BufferChanged(int);//
    void DisplayChanged(int);//
    void PChanged(int);//
    void IChanged(int);//
    void DChanged(int);//
    void PFPChanged(int);//
    void IFPChanged(int);//
    void DFPChanged(int);//
    void Load();
    void Save();
    void Reset();//
    //void Upload();
private:

    void initialiseVariables(QString);//
    void setupGraph();//
    void setupSlots();// load save upload
    enum DataIndex {state,setPoint,correction,error};
    enum PIDIndex { p, i, d};
    struct pid{
      uint16_t byte=0;
      void setExp(int);
      void setData(int);
      void setValue(double);
      int getData();
      int getExp();
      float getValue();
    };
    pid PID[3];
    double TIME_OFFSET,DISPLAY_TIME;
    int ACTIVE_WINDOW,BUFFER;
    bool  GRAPH_DISPLAY[4],AUTO_SCROLL_MODE;
    std::string SAVE_PATH,CONFIG_PATH;
    QString LABEL;
    Ui::PIDTunerWidget *ui;
};

#endif // PIDTUNERWIDGET_H
