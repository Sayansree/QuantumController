#include "quantum_controller/PIDTuner.h"

PIDTuner::PIDTuner( QWidget *parent) :
    QMainWindow(parent), ui(new Ui::PIDTuner)
{
    setup();
    loadTabs();
}
PIDTuner::~PIDTuner(){
    delete ui;
}

void PIDTuner::TabChanged(int val){
  TIME_OFFSET=T;
  TABS.at(ACTIVE_WINDOW)->tabKill();
  ACTIVE_WINDOW=val;
}

void PIDTuner::setup(){
  //LOGO_PATH = ros::package::getPath("quantum_controller") + "/utils/logo.jpeg";
  CONFIG_PATH=ros::package::getPath("quantum_controller")+"/config/";
  TIME_OFFSET=T=0;
  ACTIVE_WINDOW=0;
  ui->setupUi(this);
  connect(ui->Tuning,SIGNAL(currentChanged(int)),this,SLOT(TabChanged(int)));
}
void PIDTuner::addTab(QString label){
  TABS.push_back(new PIDTunerWidget(label,new QWidget()));
  ui->Tuning->addTab(TABS.back(),label);
}
void PIDTuner::loadTabs(){
  std::string line;
  std::vector <std::string> tokens;
  std::fstream f;
  f.open(CONFIG_PATH+"config.pid",std::ios::in);
  while (std::getline(f, line))
  {
     boost::trim(line);
     if (line[0] == '#' || line.size() == 0)
       continue;
     tokens=boost::split(tokens,line,boost::is_any_of(",:|[]()<>"));
     addTab(QString::fromStdString(tokens[0]));
  }
  f.close();
}
void PIDTuner::verifyValues(uint16_t val[], bool arr[]){

}
void PIDTuner::updateDataPoints(double val[][4], double t){
  T=t;
  TABS.at(ACTIVE_WINDOW)->updateDataPoints(val[2],t-TIME_OFFSET);
}
