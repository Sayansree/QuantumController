#include "quantum_controller/PIDTunerWidget.h"

PIDTunerWidget::PIDTunerWidget(QString title, QWidget *parent) :
    QWidget(parent), ui(new Ui::PIDTunerWidget){
    initialiseVariables(title);
    setupGraph();
    setupSlots();
    Load();
}

PIDTunerWidget::~PIDTunerWidget(){
    delete ui;
}

void PIDTunerWidget::GraphClear(){
  ui->Graph->graph(state)->data()->clear();
  ui->Graph->graph(setPoint)->data()->clear();
  ui->Graph->graph(error)->data()->clear();
  ui->Graph->graph(correction)->data()->clear();
  TIME_OFFSET+=DISPLAY_TIME;
}
void PIDTunerWidget::GraphSave(){
  int index;
  std::fstream f;
  f.open(CONFIG_PATH+"saveIndex.conf",std::ios::in|std::ios::out);
  f>>index;
  f.seekp(0);
  f<<++index<<std::endl;
  f.close();
  ui->Graph->saveBmp(QString::fromStdString(SAVE_PATH
    +std::to_string(index)+"Graph.bmp"));
}
void PIDTunerWidget::GraphAutoScroll(){
  AUTO_SCROLL_MODE=!AUTO_SCROLL_MODE;
  if(AUTO_SCROLL_MODE)
    ui->AutoScroll->setText("NoAutoScroll");
  else
    ui->AutoScroll->setText("AutoScroll");
}
void PIDTunerWidget::GraphInspect(bool val){
  ui->AutoScroll->setEnabled(!val);
  ui->ManualLabel->setEnabled(val);
  ui->Manual->setEnabled(val);
  ui->ManualValue->setEnabled(val);
  if(val){
    ui->Manual->setMaximum(DISPLAY_TIME);
    ui->Manual->setSliderPosition(DISPLAY_TIME);
  }
  AUTO_SCROLL_MODE=val;
  GraphAutoScroll();
}
void PIDTunerWidget::ValueGraph(int val){
  GRAPH_DISPLAY[state]=val;
  if(val==0)ui->Graph->graph(state)->data()->clear();
}
void PIDTunerWidget::ErrorGraph(int val){
  GRAPH_DISPLAY[error]=val;
  if(val==0)ui->Graph->graph(error)->data()->clear();
}
void PIDTunerWidget::SetpointGraph(int val){
  GRAPH_DISPLAY[setPoint]=val;
  if(val==0)ui->Graph->graph(setPoint)->data()->clear();
}
void PIDTunerWidget::CorrectionGraph(int val){
  GRAPH_DISPLAY[correction]=val;
  if(val==0)ui->Graph->graph(correction)->data()->clear();
}
void PIDTunerWidget::BufferChanged(int val){
  ui->BufferValue->setText(QString::number(val));
  BUFFER=val;
}
void PIDTunerWidget::DisplayChanged(int val){
  DISPLAY_TIME=val;
  ui->ManualValue->setText(QString::number(DISPLAY_TIME));
}
void PIDTunerWidget::PChanged(int val){
  PID[p].setData(val);
  ui->PDisp->setText(QString::number(PID[p].getValue()));
}
void PIDTunerWidget::IChanged(int val){
  PID[i].setData(val);
  ui->IDisp->setText(QString::number(PID[i].getValue()));
}
void PIDTunerWidget::DChanged(int val){
  PID[d].setData(val);
  ui->DDisp->setText(QString::number(PID[d].getValue()));
}
void PIDTunerWidget::PFPChanged(int val){
  PID[p].setExp(val);
  ui->PLC->setText(QString::number(pow(10,val)));
  ui->PMax->setText(QString::number(999*pow(10,val)));
  ui->PDisp->setText(QString::number(PID[p].getValue()));
}
void PIDTunerWidget::IFPChanged(int val){
  PID[i].setExp(val);
  ui->ILC->setText(QString::number(pow(10,val)));
  ui->IMax->setText(QString::number(999*pow(10,val)));
  ui->IDisp->setText(QString::number(PID[i].getValue()));
}
void PIDTunerWidget::DFPChanged(int val){
  PID[d].setExp(val);
  ui->DLC->setText(QString::number(pow(10,val)));
  ui->DMax->setText(QString::number(999*pow(10,val)));
  ui->DDisp->setText(QString::number(PID[d].getValue()));
}
void PIDTunerWidget::Save(){
  std::string line;
  std::vector <std::string> tokens,lines;
  std::fstream f;
  f.open(CONFIG_PATH+"config.pid",std::ios::in);
  while (std::getline(f, line))
  {
     boost::trim(line);
     lines.push_back(line+"\n");
     if (line[0] == '#' || line.size() == 0)
       continue;
     tokens=boost::split(tokens,line,boost::is_any_of(",:|[]()<>"));
     if(tokens[0]==LABEL.toStdString()){
       line=tokens[0]+"<"
       +std::to_string(PID[p].getValue())+","
       +std::to_string(PID[i].getValue())+","
       +std::to_string(PID[d].getValue())+">\n";
       lines.back()=line;
     }
   }
   f.close();
   f.open(CONFIG_PATH+"config.pid",std::ios::out);
   for(std::string l:lines)
      f<<l;
   f.close();
}
void PIDTunerWidget::Load(){
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
     if(tokens[0]==LABEL.toStdString()){
       PID[p].setValue(std::atof(tokens[1].c_str()));
       PID[i].setValue(std::atof(tokens[2].c_str()));
       PID[d].setValue(std::atof(tokens[3].c_str()));
       break;
     }
  }
  f.close();
  ui->P->setSliderPosition(PID[p].getData());
  ui->I->setSliderPosition(PID[i].getData());
  ui->D->setSliderPosition(PID[d].getData());
  ui->PFP->setSliderPosition(PID[p].getExp());
  ui->IFP->setSliderPosition(PID[i].getExp());
  ui->DFP->setSliderPosition(PID[d].getExp());
}
void PIDTunerWidget::Reset(){
  PID[p].setValue(0);
  PID[i].setValue(0);
  PID[d].setValue(0);
  ui->P->setSliderPosition(PID[p].getData());
  ui->I->setSliderPosition(PID[i].getData());
  ui->D->setSliderPosition(PID[d].getData());
  ui->PFP->setSliderPosition(PID[p].getExp());
  ui->IFP->setSliderPosition(PID[i].getExp());
  ui->DFP->setSliderPosition(PID[d].getExp());
}
void PIDTunerWidget::Upload(){

}

void PIDTunerWidget::initialiseVariables(QString label){
  SAVE_PATH = ros::package::getPath("quantum_controller")+"/saved/"+label.toStdString();
  CONFIG_PATH=ros::package::getPath("quantum_controller")+"/config/";
  LABEL=label;
  TIME_OFFSET=DISPLAY_TIME=0;BUFFER=20;
  GRAPH_DISPLAY[state]=GRAPH_DISPLAY[error]=
  GRAPH_DISPLAY[correction]=GRAPH_DISPLAY[setPoint]=true;
  AUTO_SCROLL_MODE=true;
  ui->setupUi(this);
}
void PIDTunerWidget::setupGraph(){
  QLinearGradient plotGradient;
  plotGradient.setStart(0, 0);
  plotGradient.setFinalStop(0, 350);
  plotGradient.setColorAt(0, QColor(0x45,0x5A,0x64));
  plotGradient.setColorAt(1, QColor(0x21,0x21,0x21));
  ui->Graph->setBackground(plotGradient);
  ui->Graph->legend->setVisible(true);
  ui->Graph->legend->setBrush(plotGradient);
  ui->Graph->legend->setTextColor(Qt::white);
  ui->Graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->Graph->yAxis2->setVisible(true);
  ui->Graph->yAxis2->setTickLabels(true);
  ui->Graph->xAxis->setBasePen(QPen(Qt::white, 1));
  ui->Graph->yAxis->setBasePen(QPen(Qt::white, 1));
  ui->Graph->yAxis2->setBasePen(QPen(Qt::white, 1));
  ui->Graph->xAxis->setTickPen(QPen(Qt::white, 1));
  ui->Graph->yAxis->setTickPen(QPen(Qt::white, 1));
  ui->Graph->yAxis2->setTickPen(QPen(Qt::white, 1));
  ui->Graph->xAxis->setSubTickPen(QPen(Qt::white, 1));
  ui->Graph->yAxis->setSubTickPen(QPen(Qt::white, 1));
  ui->Graph->yAxis2->setSubTickPen(QPen(Qt::white, 1));
  ui->Graph->xAxis->setTickLabelColor(Qt::white);
  ui->Graph->yAxis->setTickLabelColor(Qt::white);
  ui->Graph->yAxis2->setTickLabelColor(Qt::white);
  ui->Graph->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  ui->Graph->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  ui->Graph->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
  ui->Graph->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
  ui->Graph->xAxis->grid()->setSubGridVisible(true);
  ui->Graph->yAxis->grid()->setSubGridVisible(true);
  ui->Graph->xAxis->grid()->setZeroLinePen(Qt::NoPen);
  ui->Graph->yAxis->grid()->setZeroLinePen(QPen(Qt::white, 1));
  ui->Graph->xAxis->setLabelColor(Qt::white);
  ui->Graph->yAxis->setLabelColor(Qt::white);
  ui->Graph->yAxis2->setLabelColor(Qt::white);
  ui->Graph->xAxis->setLabel("mili seconds (ms)");
  ui->Graph->yAxis->setLabel("Value");
  ui->Graph->yAxis2->setLabel("PID corection %");
  ui->Graph->addGraph()->setName(LABEL);
  ui->Graph->addGraph()->setName("Setpoint");
  ui->Graph->addGraph()->setName("error");
  ui->Graph->addGraph()->setName("PID");
  ui->Graph->graph(state)->setPen(QPen(Qt::yellow));
  ui->Graph->graph(setPoint)->setPen(QPen(Qt::green));
  ui->Graph->graph(error)->setPen(QPen(Qt::red));
  ui->Graph->graph(correction)->setPen(QPen(Qt::cyan));

}
void PIDTunerWidget::setupSlots(){
  //graph setup slots
  connect(ui->Value,SIGNAL(stateChanged(int)),this,SLOT(ValueGraph(int)));
  connect(ui->Error,SIGNAL(stateChanged(int)),this,SLOT(ErrorGraph(int)));
  connect(ui->Setpoint,SIGNAL(stateChanged(int)),this,SLOT(SetpointGraph(int)));
  connect(ui->Correction,SIGNAL(stateChanged(int)),this,SLOT(CorrectionGraph(int)));
  connect(ui->Graph->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->Graph->yAxis2, SLOT(setRange(QCPRange)));
  connect(ui->Buffer,SIGNAL(valueChanged(int)),this,SLOT(BufferChanged(int)));
  connect(ui->Manual,SIGNAL(valueChanged(int)),this,SLOT(DisplayChanged(int)));
  connect(ui->Save, SIGNAL(pressed()), this, SLOT(GraphSave()));
  connect(ui->Clear, SIGNAL(pressed()), this, SLOT(GraphClear()));
  connect(ui->AutoScroll,SIGNAL(pressed()), this, SLOT(GraphAutoScroll()));
  connect(ui->Inspect, SIGNAL(clicked(bool)), this, SLOT(GraphInspect(bool)));
  //tuner setup slots
  connect(ui->P,SIGNAL(valueChanged(int)),this,SLOT(PChanged(int)));
  connect(ui->I,SIGNAL(valueChanged(int)),this,SLOT(IChanged(int)));
  connect(ui->D,SIGNAL(valueChanged(int)),this,SLOT(DChanged(int)));
  connect(ui->PFP,SIGNAL(valueChanged(int)),this,SLOT(PFPChanged(int)));
  connect(ui->IFP,SIGNAL(valueChanged(int)),this,SLOT(IFPChanged(int)));
  connect(ui->DFP,SIGNAL(valueChanged(int)),this,SLOT(DFPChanged(int)));
  connect(ui->SavePID, SIGNAL(pressed()), this, SLOT(Save()));
  connect(ui->LoadPID, SIGNAL(pressed()), this, SLOT(Load()));
  connect(ui->ResetPID, SIGNAL(pressed()), this, SLOT(Reset()));
  connect(ui->UploadPID, SIGNAL(pressed()), this, SLOT(Upload()));
}

void PIDTunerWidget::updateDataPoints(double val[], double t){
  t=t-TIME_OFFSET;
  if(GRAPH_DISPLAY[state])
    ui->Graph->graph(state)->addData(t, val[state]);
  if(GRAPH_DISPLAY[setPoint])
    ui->Graph->graph(setPoint)->addData(t, val[setPoint]);
  if(GRAPH_DISPLAY[correction])
    ui->Graph->graph(correction)->addData(t, val[correction]);
  if(GRAPH_DISPLAY[error])
    ui->Graph->graph(error)->addData(t,val[state]-val[setPoint]);
  if(AUTO_SCROLL_MODE)
    DISPLAY_TIME=t;
  if(GRAPH_DISPLAY[state]|GRAPH_DISPLAY[setPoint]|GRAPH_DISPLAY[error]|GRAPH_DISPLAY[correction]){
    ui->Graph->xAxis->setRange(DISPLAY_TIME-BUFFER,DISPLAY_TIME+BUFFER/5.0);
    ui->Graph->yAxis->rescale();
    ui->Graph->yAxis->scaleRange(1.5);
    ui->Graph->replot();
  }
}
void PIDTunerWidget::tabKill(){
  GraphClear();
  GraphInspect(false);
  DISPLAY_TIME=TIME_OFFSET=0;
}

void PIDTunerWidget::pid::setExp(int val){
  val+=16;
  byte=(uint16_t)(byte&0x83FF|val<<10);
}
void PIDTunerWidget::pid::setData(int val){
  byte=(uint16_t)(byte&0x7C00|0x03FF&val);
  if(val<0)
  byte|=1<<15;
}
void PIDTunerWidget::pid::setValue(double val){
  if(val==0){
    setData(0);
    setExp(0);
    return;
  }
  int exp= (int)log10(abs(val)) -2;
  int data=(int)(val*pow(10,-exp));
  setData(data);
  setExp(exp);
}
int PIDTunerWidget::pid::getData(){
  int data= byte&0x03FF;
  return(byte>>15==1)?-data:data;
}
int PIDTunerWidget::pid::getExp(){
  int exp= (byte&0x7C00)>>10;
  return exp-16;
}
float PIDTunerWidget::pid::getValue(){
  return getData()*pow(10,getExp());
}
