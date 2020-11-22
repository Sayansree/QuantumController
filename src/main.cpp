#include "quantum_controller/PIDTuner.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PIDTuner *pidTuner=pidTuner=new PIDTuner();
    pidTuner->show();
    //a.exec();
    float t=0;
    double v[3][4];
    while(pidTuner->isVisible())
    {
      t+=.1;
      v[2][0]=v[1][0]=v[0][0]=2*cos(t);
      v[2][1]=v[1][1]=v[0][1]=sin(t);
      v[2][2]=v[1][2]=v[0][2]=cos(t);
      v[2][3]=v[1][3]=v[0][3]=2*sin(t);
      pidTuner->updateDataPoints(v,t);
      a.processEvents();
    }
    return 0;
}
