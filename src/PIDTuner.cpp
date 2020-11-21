#include "quantum_controller/PIDTuner.h"

PIDTuner::PIDTuner( QWidget *parent) :
    QMainWindow(parent), ui(new Ui::PIDTuner)
{
    initialiseVariables();
    setupSlots();
    TABS.push_back(new PIDTunerWidget(QString::fromStdString("Pitch"),new QWidget()));
    ui->Tuning->addTab(TABS.back(),"Pitch");
    TABS.push_back(new PIDTunerWidget(QString::fromStdString("Roll"),new QWidget()));
    ui->Tuning->addTab(TABS.back(),"Roll");
    ui->Tuning->setCurrentIndex(0);

}
PIDTuner::~PIDTuner(){
    delete ui;
}
//
// void PIDTuner::PitchLoad(){
//   uint16_t d;
//   int i=0;
//   std::fstream f;
//   f.open(CONFIG_PATH+"config.pid",std::ios::in);
//   while(i++<pitchP)f>>d;
//   f>>PID[pitchP].byte;
//   f>>PID[pitchI].byte;
//   f>>PID[pitchD].byte;
//   f.close();
//   ui->PitchP->setSliderPosition(PID[pitchP].getData());
//   ui->PitchI->setSliderPosition(PID[pitchI].getData());
//   ui->PitchD->setSliderPosition(PID[pitchD].getData());
//   ui->PitchPFP->setSliderPosition(PID[pitchP].getExp());
//   ui->PitchIFP->setSliderPosition(PID[pitchI].getExp());
//   ui->PitchDFP->setSliderPosition(PID[pitchD].getExp());
// }
// void PIDTuner::PitchSave(){
//   uint16_t d;
//   int i=0;
//   std::fstream f;
//   f.open(CONFIG_PATH+"config.pid",std::ios::in|std::ios::out);
//   while(i++<pitchP)f>>d;
//   f<<PID[pitchP].byte<<std::endl;
//   f<<PID[pitchI].byte<<std::endl;
//   f<<PID[pitchD].byte<<std::endl;
//   f.close();
// }
// void PIDTuner::PitchReset(){
//   PID[pitchP].setValue(0);
//   PID[pitchI].setValue(0);
//   PID[pitchD].setValue(0);
//   ui->PitchP->setSliderPosition(PID[pitchP].getData());
//   ui->PitchI->setSliderPosition(PID[pitchI].getData());
//   ui->PitchD->setSliderPosition(PID[pitchD].getData());
//   ui->PitchPFP->setSliderPosition(PID[pitchP].getExp());
//   ui->PitchIFP->setSliderPosition(PID[pitchI].getExp());
//   ui->PitchDFP->setSliderPosition(PID[pitchD].getExp());
// }


void PIDTuner::TabChanged(int val){
  // PitchGraphClear();
  // PitchGraphInspect(false);
  // RollGraphClear();
  // RollGraphInspect(false);
  // YawGraphClear();
  // YawGraphInspect(false);
  TIME_OFFSET=T;
  TABS.at(val)->tabKill();
  ACTIVE_WINDOW=val;

}
//void PIDTuner::Upload(){
//}


void PIDTuner::initialiseVariables(){
  // LOGO_PATH = ros::package::getPath("quantum_controller") + "/utils/logo.jpeg";
  // SAVE_PATH = ros::package::getPath("quantum_controller")+"/saved/";
  // CONFIG_PATH=ros::package::getPath("quantum_controller")+"/config/";
  TIME_OFFSET=T=0;
  ui->setupUi(this);
}
void PIDTuner::setupSlots(){
  connect(ui->Tuning,SIGNAL(currentChanged(int)),this,SLOT(TabChanged(int)));

}

void PIDTuner::verifyValues(uint16_t val[], bool arr[]){
  // for(int i=0;i<9;i++)
  //   arr[i]=PID[i].byte==val[i];
}
void PIDTuner::updateDataPoints(double val[][4], double t){
  T=t;
  TABS.at(ACTIVE_WINDOW)->updateDataPoints(val[2],t-TIME_OFFSET);

}

void PIDTuner::pid::setExp(int val){
  val+=16;
  byte=(uint16_t)(byte&0x83FF|val<<10);
}
void PIDTuner::pid::setData(int val){
  byte=(uint16_t)(byte&0x7C00|0x03FF&val);
  if(val<0)
  byte|=1<<15;
}
void PIDTuner::pid::setValue(float val){
  if(val==0){
    setData(0);
    setExp(0);
    return;
  }
  int exp= (int)log10(abs(val)) -2;
  int data=(int)(val*pow(10,exp));
  setData(data);
  setExp(exp);
}
int PIDTuner::pid::getData(){
  int data= byte&0x03FF;
  return(byte>>15==1)?-data:data;
}
int PIDTuner::pid::getExp(){
  int exp= (byte&0x7C00)>>10;
  return exp-16;
}
float PIDTuner::pid::getValue(){
  return getData()*pow(10,getExp());
}
