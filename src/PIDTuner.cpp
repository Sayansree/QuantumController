#include "quantum_controller/PIDTuner.h"

PIDTuner::PIDTuner( QWidget *parent) :
    QMainWindow(parent), ui(new Ui::PIDTuner)
{
    LOGO_PATH = ros::package::getPath("quantum_controller") + "/utils/logo.jpeg";
    SAVE_PATH = ros::package::getPath("quantum_controller")+"/saved/";
    CONFIG_PATH=ros::package::getPath("quantum_controller")+"/config/";

    ui->setupUi(this);
    timer = new QTimer(this);
    timer->start(10);
    t=0;BUFFER=20;DISPLAY_TIME=0;
    AUTO_SCROLL_MODE=true;
    PITCH_GRAPH_DISPLAY[state]=PITCH_GRAPH_DISPLAY[error]=
    PITCH_GRAPH_DISPLAY[correction]=PITCH_GRAPH_DISPLAY[setPoint]=true;
    connect(timer, SIGNAL(timeout()),this,SLOT(loop()));
    setupSlots();
    setupGraph(ui->graphPitch);
    setupGraph(ui->graphRoll);
    setupGraph(ui->graphYaw);
    PitchLoad();
    RollLoad();
}
PIDTuner::~PIDTuner(){
    delete ui;
}
void PIDTuner::setupSlots(){
  connect(ui->Tuning,SIGNAL(currentChanged(int)),this,SLOT(TabChanged(int)));

  connect(ui->Pitch,SIGNAL(stateChanged(int)),this,SLOT(PitchGraph(int)));
  connect(ui->PitchError,SIGNAL(stateChanged(int)),this,SLOT(PitchErrorGraph(int)));
  connect(ui->PitchSetpoint,SIGNAL(stateChanged(int)),this,SLOT(PitchSetpointGraph(int)));
  connect(ui->PitchCorrection,SIGNAL(stateChanged(int)),this,SLOT(PitchCorrectionGraph(int)));
  connect(ui->graphPitch->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->graphPitch->yAxis2, SLOT(setRange(QCPRange)));
  connect(ui->PitchBuffer,SIGNAL(valueChanged(int)),this,SLOT(PitchBufferChanged(int)));
  connect(ui->PitchManual,SIGNAL(valueChanged(int)),this,SLOT(PitchDisplayChanged(int)));
  connect(ui->PitchSave, SIGNAL(pressed()), this, SLOT(PitchGraphSave()));
  connect(ui->PitchClear, SIGNAL(pressed()), this, SLOT(PitchGraphClear()));
  connect(ui->PitchAutoScroll,SIGNAL(pressed()), this, SLOT(PitchGraphAutoScroll()));
  connect(ui->PitchInspect, SIGNAL(clicked(bool)), this, SLOT(PitchGraphInspect(bool)));

  connect(ui->PitchP,SIGNAL(valueChanged(int)),this,SLOT(PitchPChanged(int)));
  connect(ui->PitchI,SIGNAL(valueChanged(int)),this,SLOT(PitchIChanged(int)));
  connect(ui->PitchD,SIGNAL(valueChanged(int)),this,SLOT(PitchDChanged(int)));
  connect(ui->PitchPFP,SIGNAL(valueChanged(int)),this,SLOT(PitchPFPChanged(int)));
  connect(ui->PitchIFP,SIGNAL(valueChanged(int)),this,SLOT(PitchIFPChanged(int)));
  connect(ui->PitchDFP,SIGNAL(valueChanged(int)),this,SLOT(PitchDFPChanged(int)));
  connect(ui->PitchSavePID, SIGNAL(pressed()), this, SLOT(PitchSave()));
  connect(ui->PitchLoadPID, SIGNAL(pressed()), this, SLOT(PitchLoad()));
  connect(ui->PitchResetPID, SIGNAL(pressed()), this, SLOT(PitchReset()));
  connect(ui->PitchUploadPID, SIGNAL(pressed()), this, SLOT(PitchUpload()));

  //roll setup slots
  connect(ui->Roll,SIGNAL(stateChanged(int)),this,SLOT(RollGraph(int)));
  connect(ui->RollError,SIGNAL(stateChanged(int)),this,SLOT(RollErrorGraph(int)));
  connect(ui->RollSetpoint,SIGNAL(stateChanged(int)),this,SLOT(RollSetpointGraph(int)));
  connect(ui->RollCorrection,SIGNAL(stateChanged(int)),this,SLOT(RollCorrectionGraph(int)));
  connect(ui->graphRoll->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->graphRoll->yAxis2, SLOT(setRange(QCPRange)));
  connect(ui->RollBuffer,SIGNAL(valueChanged(int)),this,SLOT(RollBufferChanged(int)));
  connect(ui->RollManual,SIGNAL(valueChanged(int)),this,SLOT(RollDisplayChanged(int)));
  connect(ui->RollSave, SIGNAL(pressed()), this, SLOT(RollGraphSave()));
  connect(ui->RollClear, SIGNAL(pressed()), this, SLOT(RollGraphClear()));
  connect(ui->RollAutoScroll,SIGNAL(pressed()), this, SLOT(RollGraphAutoScroll()));
  connect(ui->RollInspect, SIGNAL(clicked(bool)), this, SLOT(RollGraphInspect(bool)));

  connect(ui->RollP,SIGNAL(valueChanged(int)),this,SLOT(RollPChanged(int)));
  connect(ui->RollI,SIGNAL(valueChanged(int)),this,SLOT(RollIChanged(int)));
  connect(ui->RollD,SIGNAL(valueChanged(int)),this,SLOT(RollDChanged(int)));
  connect(ui->RollPFP,SIGNAL(valueChanged(int)),this,SLOT(RollPFPChanged(int)));
  connect(ui->RollIFP,SIGNAL(valueChanged(int)),this,SLOT(RollIFPChanged(int)));
  connect(ui->RollDFP,SIGNAL(valueChanged(int)),this,SLOT(RollDFPChanged(int)));
  connect(ui->RollSavePID, SIGNAL(pressed()), this, SLOT(RollSave()));
  connect(ui->RollLoadPID, SIGNAL(pressed()), this, SLOT(RollLoad()));
  connect(ui->RollResetPID, SIGNAL(pressed()), this, SLOT(RollReset()));
  connect(ui->RollUploadPID, SIGNAL(pressed()), this, SLOT(RollUpload()));

  //Yaw setup slots
  connect(ui->Yaw,SIGNAL(stateChanged(int)),this,SLOT(YawGraph(int)));
  connect(ui->YawError,SIGNAL(stateChanged(int)),this,SLOT(YawErrorGraph(int)));
  connect(ui->YawSetpoint,SIGNAL(stateChanged(int)),this,SLOT(YawSetpointGraph(int)));
  connect(ui->YawCorrection,SIGNAL(stateChanged(int)),this,SLOT(YawCorrectionGraph(int)));
  connect(ui->graphYaw->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->graphYaw->yAxis2, SLOT(setRange(QCPRange)));
  connect(ui->YawBuffer,SIGNAL(valueChanged(int)),this,SLOT(YawBufferChanged(int)));
  connect(ui->YawManual,SIGNAL(valueChanged(int)),this,SLOT(YawDisplayChanged(int)));
  connect(ui->YawSave, SIGNAL(pressed()), this, SLOT(YawGraphSave()));
  connect(ui->YawClear, SIGNAL(pressed()), this, SLOT(YawGraphClear()));
  connect(ui->YawAutoScroll,SIGNAL(pressed()), this, SLOT(YawGraphAutoScroll()));
  connect(ui->YawInspect, SIGNAL(clicked(bool)), this, SLOT(YawGraphInspect(bool)));

  connect(ui->YawP,SIGNAL(valueChanged(int)),this,SLOT(YawPChanged(int)));
  connect(ui->YawI,SIGNAL(valueChanged(int)),this,SLOT(YawIChanged(int)));
  connect(ui->YawD,SIGNAL(valueChanged(int)),this,SLOT(YawDChanged(int)));
  connect(ui->YawPFP,SIGNAL(valueChanged(int)),this,SLOT(YawPFPChanged(int)));
  connect(ui->YawIFP,SIGNAL(valueChanged(int)),this,SLOT(YawIFPChanged(int)));
  connect(ui->YawDFP,SIGNAL(valueChanged(int)),this,SLOT(YawDFPChanged(int)));
  connect(ui->YawSavePID, SIGNAL(pressed()), this, SLOT(YawSave()));
  connect(ui->YawLoadPID, SIGNAL(pressed()), this, SLOT(YawLoad()));
  connect(ui->YawResetPID, SIGNAL(pressed()), this, SLOT(YawReset()));
  connect(ui->YawUploadPID, SIGNAL(pressed()), this, SLOT(YawUpload()));
}
void PIDTuner::TabChanged(int val){
  PitchGraphClear();
  PitchGraphInspect(false);
  RollGraphClear();
  RollGraphInspect(false);
  ACTIVE_WINDOW=val;
  AUTO_SCROLL_MODE=true;
}

void PIDTuner::PitchGraph(int val){
  PITCH_GRAPH_DISPLAY[state]=val;
  if(val==0)ui->graphPitch->graph(state)->data()->clear();
}
void PIDTuner::PitchGraphClear(){
  clearGraph(ui->graphPitch);
}
void PIDTuner::PitchGraphSave(){
  int index;
  std::fstream f;
  f.open(CONFIG_PATH+"saveIndex.conf",std::ios::in|std::ios::out);
  f>>index;
  f.seekp(0);
  f<<++index<<std::endl;
  f.close();
  ui->graphPitch->saveBmp(QString::fromStdString(SAVE_PATH
    +std::to_string(index)+"PitchGraph.bmp"));
}
void PIDTuner::PitchGraphAutoScroll(){
  AUTO_SCROLL_MODE=!AUTO_SCROLL_MODE;
  if(AUTO_SCROLL_MODE)
    ui->PitchAutoScroll->setText("NoAutoScroll");
  else
    ui->PitchAutoScroll->setText("AutoScroll");
}
void PIDTuner::PitchGraphInspect(bool val){
  ui->PitchAutoScroll->setEnabled(!val);
  ui->PitchManualLabel->setEnabled(val);
  ui->PitchManual->setEnabled(val);
  ui->PitchManualValue->setEnabled(val);
  if(val){
    ui->PitchManual->setMaximum(t);
    ui->PitchManual->setSliderPosition(t);
    DISPLAY_TIME=t;
  }
  AUTO_SCROLL_MODE=val;
  PitchGraphAutoScroll();
}
void PIDTuner::PitchErrorGraph(int val){
  PITCH_GRAPH_DISPLAY[error]=val;
  if(val==0)ui->graphPitch->graph(error)->data()->clear();
}
void PIDTuner::PitchSetpointGraph(int val){
  PITCH_GRAPH_DISPLAY[setPoint]=val;
  if(val==0)ui->graphPitch->graph(setPoint)->data()->clear();
}
void PIDTuner::PitchCorrectionGraph(int val){
  PITCH_GRAPH_DISPLAY[correction]=val;
  if(val==0)ui->graphPitch->graph(correction)->data()->clear();
}
void PIDTuner::PitchBufferChanged(int val){
  ui->PitchBufferValue->setText(QString::number(val));
  BUFFER=val;
}
void PIDTuner::PitchDisplayChanged(int val){
  DISPLAY_TIME=val;
  ui->PitchManualValue->setText(QString::number(DISPLAY_TIME));
}
void PIDTuner::PitchPChanged(int val){
  PID[pitchP].setData(val);
  ui->PitchPDisp->setText(QString::number(PID[pitchP].getValue()));
}
void PIDTuner::PitchIChanged(int val){
  PID[pitchI].setData(val);
  ui->PitchIDisp->setText(QString::number(PID[pitchI].getValue()));
}
void PIDTuner::PitchDChanged(int val){
  PID[pitchD].setData(val);
  ui->PitchDDisp->setText(QString::number(PID[pitchD].getValue()));
}
void PIDTuner::PitchPFPChanged(int val){
  PID[pitchP].setExp(val);
  ui->PitchPLC->setText(QString::number(pow(10,val)));
  ui->PitchPMax->setText(QString::number(999*pow(10,val)));
  ui->PitchPDisp->setText(QString::number(PID[pitchP].getValue()));
}
void PIDTuner::PitchIFPChanged(int val){
  PID[pitchI].setExp(val);
  ui->PitchILC->setText(QString::number(pow(10,val)));
  ui->PitchIMax->setText(QString::number(999*pow(10,val)));
  ui->PitchIDisp->setText(QString::number(PID[pitchI].getValue()));
}
void PIDTuner::PitchDFPChanged(int val){
  PID[pitchD].setExp(val);
  ui->PitchDLC->setText(QString::number(pow(10,val)));
  ui->PitchDMax->setText(QString::number(999*pow(10,val)));
  ui->PitchDDisp->setText(QString::number(PID[pitchD].getValue()));
}
void PIDTuner::PitchLoad(){
  uint16_t d;
  int i=0;
  std::fstream f;
  f.open(CONFIG_PATH+"config.pid",std::ios::in);
  while(i++<pitchP)f>>d;
  f>>PID[pitchP].byte;
  f>>PID[pitchI].byte;
  f>>PID[pitchD].byte;
  f.close();
  ui->PitchP->setSliderPosition(PID[pitchP].getData());
  ui->PitchI->setSliderPosition(PID[pitchI].getData());
  ui->PitchD->setSliderPosition(PID[pitchD].getData());
  ui->PitchPFP->setSliderPosition(PID[pitchP].getExp());
  ui->PitchIFP->setSliderPosition(PID[pitchI].getExp());
  ui->PitchDFP->setSliderPosition(PID[pitchD].getExp());
}
void PIDTuner::PitchSave(){
  uint16_t d;
  int i=0;
  std::fstream f;
  f.open(CONFIG_PATH+"config.pid",std::ios::in|std::ios::out);
  while(i++<pitchP)f>>d;
  f<<PID[pitchP].byte<<std::endl;
  f<<PID[pitchI].byte<<std::endl;
  f<<PID[pitchD].byte<<std::endl;
  f.close();
}
void PIDTuner::PitchReset(){
  PID[pitchP].setValue(0);
  PID[pitchI].setValue(0);
  PID[pitchD].setValue(0);
  ui->PitchP->setSliderPosition(PID[pitchP].getData());
  ui->PitchI->setSliderPosition(PID[pitchI].getData());
  ui->PitchD->setSliderPosition(PID[pitchD].getData());
  ui->PitchPFP->setSliderPosition(PID[pitchP].getExp());
  ui->PitchIFP->setSliderPosition(PID[pitchI].getExp());
  ui->PitchDFP->setSliderPosition(PID[pitchD].getExp());
}
void PIDTuner::PitchUpload(){

} //to do : add upload options

void PIDTuner::RollGraph(int val){
  ROLL_GRAPH_DISPLAY[state]=val;
  if(val==0)ui->graphRoll->graph(state)->data()->clear();
}
void PIDTuner::RollGraphClear(){
  clearGraph(ui->graphRoll);
}
void PIDTuner::RollGraphSave(){
  int index;
  std::fstream f;
  f.open(CONFIG_PATH+"saveIndex.conf",std::ios::in|std::ios::out);
  f>>index;
  f.seekp(0);
  f<<++index<<std::endl;
  f.close();
  ui->graphRoll->saveBmp(QString::fromStdString(SAVE_PATH
    +std::to_string(index)+"RollGraph.bmp"));
}
void PIDTuner::RollGraphAutoScroll(){
  AUTO_SCROLL_MODE=!AUTO_SCROLL_MODE;
  if(AUTO_SCROLL_MODE)
    ui->RollAutoScroll->setText("NoAutoScroll");
  else
    ui->RollAutoScroll->setText("AutoScroll");
}
void PIDTuner::RollGraphInspect(bool val){
  ui->RollAutoScroll->setEnabled(!val);
  ui->RollManualLabel->setEnabled(val);
  ui->RollManual->setEnabled(val);
  ui->RollManualValue->setEnabled(val);
  if(val){
    ui->RollManual->setMaximum(t);
    ui->RollManual->setSliderPosition(t);
    DISPLAY_TIME=t;
  }
  AUTO_SCROLL_MODE=val;
  RollGraphAutoScroll();
}
void PIDTuner::RollErrorGraph(int val){
  ROLL_GRAPH_DISPLAY[error]=val;
  if(val==0)ui->graphRoll->graph(error)->data()->clear();
}
void PIDTuner::RollSetpointGraph(int val){
  ROLL_GRAPH_DISPLAY[setPoint]=val;
  if(val==0)ui->graphRoll->graph(setPoint)->data()->clear();
}
void PIDTuner::RollCorrectionGraph(int val){
  ROLL_GRAPH_DISPLAY[correction]=val;
  if(val==0)ui->graphRoll->graph(correction)->data()->clear();
}
void PIDTuner::RollBufferChanged(int val){
  ui->RollBufferValue->setText(QString::number(val));
  BUFFER=val;
}
void PIDTuner::RollDisplayChanged(int val){
  DISPLAY_TIME=val;
  ui->RollManualValue->setText(QString::number(DISPLAY_TIME));
}
void PIDTuner::RollPChanged(int val){
  PID[rollP].setData(val);
  ui->RollPDisp->setText(QString::number(PID[rollP].getValue()));
}
void PIDTuner::RollIChanged(int val){
  PID[rollI].setData(val);
  ui->RollIDisp->setText(QString::number(PID[rollI].getValue()));
}
void PIDTuner::RollDChanged(int val){
  PID[rollD].setData(val);
  ui->RollDDisp->setText(QString::number(PID[rollD].getValue()));
}
void PIDTuner::RollPFPChanged(int val){
  PID[rollP].setExp(val);
  ui->RollPLC->setText(QString::number(pow(10,val)));
  ui->RollPMax->setText(QString::number(999*pow(10,val)));
  ui->RollPDisp->setText(QString::number(PID[rollP].getValue()));
}
void PIDTuner::RollIFPChanged(int val){
  PID[rollI].setExp(val);
  ui->RollILC->setText(QString::number(pow(10,val)));
  ui->RollIMax->setText(QString::number(999*pow(10,val)));
  ui->RollIDisp->setText(QString::number(PID[rollI].getValue()));
}
void PIDTuner::RollDFPChanged(int val){
  PID[rollD].setExp(val);
  ui->RollDLC->setText(QString::number(pow(10,val)));
  ui->RollDMax->setText(QString::number(999*pow(10,val)));
  ui->RollDDisp->setText(QString::number(PID[rollD].getValue()));
}
void PIDTuner::RollLoad(){
  uint16_t d;
  int i=0;
  std::fstream f;
  f.open(CONFIG_PATH+"config.pid",std::ios::in);
  while(i++<rollP)f>>d;
  f>>PID[rollP].byte;
  f>>PID[rollI].byte;
  f>>PID[rollD].byte;
  f.close();
  ui->RollP->setSliderPosition(PID[rollP].getData());
  ui->RollI->setSliderPosition(PID[rollI].getData());
  ui->RollD->setSliderPosition(PID[rollD].getData());
  ui->RollPFP->setSliderPosition(PID[rollP].getExp());
  ui->RollIFP->setSliderPosition(PID[rollI].getExp());
  ui->RollDFP->setSliderPosition(PID[rollD].getExp());
}
void PIDTuner::RollSave(){
  uint16_t d;
  int i=0;
  std::fstream f;
  f.open(CONFIG_PATH+"config.pid",std::ios::in|std::ios::out);
  while(i++<rollP)f>>d;
  f<<std::endl;
  f<<PID[rollP].byte<<std::endl;
  f<<PID[rollI].byte<<std::endl;
  f<<PID[rollD].byte<<std::endl;
  f.close();
}
void PIDTuner::RollReset(){
  PID[rollP].setValue(0);
  PID[rollI].setValue(0);
  PID[rollD].setValue(0);
  ui->RollP->setSliderPosition(PID[rollP].getData());
  ui->RollI->setSliderPosition(PID[rollI].getData());
  ui->RollD->setSliderPosition(PID[rollD].getData());
  ui->RollPFP->setSliderPosition(PID[rollP].getExp());
  ui->RollIFP->setSliderPosition(PID[rollI].getExp());
  ui->RollDFP->setSliderPosition(PID[rollD].getExp());
}
void PIDTuner::RollUpload(){

} //to do : add upload options
void PIDTuner::setupGraph(QCustomPlot* graph){
  QLinearGradient plotGradient;
  plotGradient.setStart(0, 0);
  plotGradient.setFinalStop(0, 350);
  plotGradient.setColorAt(0, QColor(0x45,0x5A,0x64));
  plotGradient.setColorAt(1, QColor(0x21,0x21,0x21));
  graph->setBackground(plotGradient);
  graph->legend->setVisible(true);
  graph->legend->setBrush(plotGradient);
  graph->legend->setTextColor(Qt::white);
  graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  graph->yAxis2->setVisible(true);
  graph->yAxis2->setTickLabels(true);
  graph->xAxis->setBasePen(QPen(Qt::white, 1));
  graph->yAxis->setBasePen(QPen(Qt::white, 1));
  graph->yAxis2->setBasePen(QPen(Qt::white, 1));
  graph->xAxis->setTickPen(QPen(Qt::white, 1));
  graph->yAxis->setTickPen(QPen(Qt::white, 1));
  graph->yAxis2->setTickPen(QPen(Qt::white, 1));
  graph->xAxis->setSubTickPen(QPen(Qt::white, 1));
  graph->yAxis->setSubTickPen(QPen(Qt::white, 1));
  graph->yAxis2->setSubTickPen(QPen(Qt::white, 1));
  graph->xAxis->setTickLabelColor(Qt::white);
  graph->yAxis->setTickLabelColor(Qt::white);
  graph->yAxis2->setTickLabelColor(Qt::white);
  graph->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  graph->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  graph->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
  graph->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
  graph->xAxis->grid()->setSubGridVisible(true);
  graph->yAxis->grid()->setSubGridVisible(true);
  graph->xAxis->grid()->setZeroLinePen(Qt::NoPen);
  graph->yAxis->grid()->setZeroLinePen(QPen(Qt::white, 1));
  graph->xAxis->setLabelColor(Qt::white);
  graph->yAxis->setLabelColor(Qt::white);
  graph->yAxis2->setLabelColor(Qt::white);
  graph->xAxis->setLabel("mili seconds (ms)");
  graph->yAxis->setLabel("degrees");
  graph->yAxis2->setLabel("PID corection %");
  graph->addGraph()->setName("Pitch");
  graph->addGraph()->setName("Setpoint");
  graph->addGraph()->setName("error");
  graph->addGraph()->setName("PID");
  graph->graph(state)->setPen(QPen(Qt::yellow));
  graph->graph(setPoint)->setPen(QPen(Qt::green));
  graph->graph(error)->setPen(QPen(Qt::red));
  graph->graph(correction)->setPen(QPen(Qt::cyan));

}
void PIDTuner::clearGraph(QCustomPlot* plt){
  plt->graph(state)->data()->clear();
  plt->graph(setPoint)->data()->clear();
  plt->graph(error)->data()->clear();
  plt->graph(correction)->data()->clear();
  t=0;
}

void PIDTuner::loop(){
  t++;
  if(PITCH_GRAPH_DISPLAY[state])
    ui->graphPitch->graph(state)->addData(t/10.0, sin(t/10.0));
  if(PITCH_GRAPH_DISPLAY[setPoint])
    ui->graphPitch->graph(setPoint)->addData(t/10.0, cos(t/10.0));
  if(PITCH_GRAPH_DISPLAY[error])
    ui->graphPitch->graph(error)->addData(t/10.0, 2*sin(t/10.0));
  if(PITCH_GRAPH_DISPLAY[correction])
    ui->graphPitch->graph(correction)->addData(t/10.0, 2*cos(t/10.0));
  if(AUTO_SCROLL_MODE)
    DISPLAY_TIME=t;
  ui->graphPitch->xAxis->setRange(DISPLAY_TIME/10.0-BUFFER,DISPLAY_TIME/10.0+BUFFER/5.0);
  ui->graphPitch->yAxis->rescale();
  ui->graphPitch->yAxis->scaleRange(1.5);
  ui->graphPitch->replot();
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
