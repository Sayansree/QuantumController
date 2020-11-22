#include "quantum_controller/PIDTuner.h"

PIDTuner::PIDTuner( QWidget *parent) :
    QMainWindow(parent), ui(new Ui::PIDTuner)
{
    setup();
    addTab("Pitch");
    addTab("Roll");
    addTab("Yaw");
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
  TIME_OFFSET=T=0;
  ACTIVE_WINDOW=0;
  ui->setupUi(this);
  connect(ui->Tuning,SIGNAL(currentChanged(int)),this,SLOT(TabChanged(int)));
}
void PIDTuner::addTab(QString label){
  TABS.push_back(new PIDTunerWidget(label,new QWidget()));
  ui->Tuning->addTab(TABS.back(),label);
}
void PIDTuner::verifyValues(uint16_t val[], bool arr[]){

}
void PIDTuner::updateDataPoints(double val[][4], double t){
  T=t;
  TABS.at(ACTIVE_WINDOW)->updateDataPoints(val[2],t-TIME_OFFSET);
}
