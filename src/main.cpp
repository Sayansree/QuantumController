#include "quantum_controller/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //ros::init(argc, argv, "tiburon_gui");
    //ros::NodeHandle nh;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    while(w.isVisible())
    {
      //ros::spinOnce();
      a.processEvents();
    }
    return 0;
}
