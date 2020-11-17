#include "quantum_controller/PIDTuner.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //ros::init(argc, argv, "tiburon_gui");
    //ros::NodeHandle nh;
    QApplication a(argc, argv);
    PIDTuner w;
    w.show();
    while(w.isVisible())
    {
      //ros::spinOnce();
      a.processEvents();
    }
    return 0;
}
