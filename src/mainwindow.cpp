#include "quantum_controller/mainwindow.h"

MainWindow::MainWindow(ros::NodeHandle _nh, QWidget *parent) :
    QMainWindow(parent), nh(_nh), ui(new Ui::MainWindow)
{
    LOGO_PATH = ros::package::getPath("quantum_controller") + "/utils/logo.jpeg";
    SAVE_PATH = ros::package::getPath("quantum_controller")+"/saved/";
    ui->setupUi(this);
    //this->setStyleSheet("background-color: #212121;");
    timer = new QTimer(this);
    timer->start(10);
    t=0;BUFFER=20;DISPLAY_TIME=0;AUTO_SCROLL_MODE=true;
    PITCH_GRAPH_DISPLAY[state]=PITCH_GRAPH_DISPLAY[error]=
    PITCH_GRAPH_DISPLAY[correction]=PITCH_GRAPH_DISPLAY[setPoint]=true;
    connect(timer, SIGNAL(timeout()),this,SLOT(loop()));
    setupSlots();
    setupGraph(ui->graphPitch);
}
MainWindow::~MainWindow(){
    delete ui;
}
void MainWindow::setupSlots(){
  connect(ui->Pitch,SIGNAL(stateChanged(int)),this,SLOT(PitchGraph(int)));
  connect(ui->PitchError,SIGNAL(stateChanged(int)),this,SLOT(PitchErrorGraph(int)));
  connect(ui->PitchSetpoint,SIGNAL(stateChanged(int)),this,SLOT(PitchSetpointGraph(int)));
  connect(ui->PitchCorrection,SIGNAL(stateChanged(int)),this,SLOT(PitchCorrectionGraph(int)));
  connect(ui->graphPitch->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->graphPitch->yAxis2, SLOT(setRange(QCPRange)));
  connect(ui->PitchBuffer,SIGNAL(valueChanged(int)),this,SLOT(PitchBufferChanged(int)));
  connect(ui->PitchManual,SIGNAL(valueChanged(int)),this,SLOT(PitchDisplayChanged(int)));
  connect(ui->PitchSave, SIGNAL(pressed()), this, SLOT(PitchGraphSave()));
  connect(ui->PitchClear, SIGNAL(pressed()), this, SLOT(PitchGraphClear()));
  connect(ui->PitchAutoScroll, SIGNAL(pressed()), this, SLOT(PitchGraphAutoScroll()));
  connect(ui->PitchInspect, SIGNAL(clicked(bool)), this, SLOT(PitchGraphInspect(bool)));
}
void MainWindow::PitchGraph(int val){
  PITCH_GRAPH_DISPLAY[state]=val;
  if(val==0)ui->graphPitch->graph(state)->data()->clear();
}
void MainWindow::PitchGraphClear(){
  clearGraph(ui->graphPitch);
}
void MainWindow::PitchGraphSave(){
  ui->graphPitch->saveBmp(QString::fromStdString(SAVE_PATH+"graph.bmp"));
}
void MainWindow::PitchGraphAutoScroll(){
  AUTO_SCROLL_MODE=!AUTO_SCROLL_MODE;
  if(AUTO_SCROLL_MODE)
    ui->PitchAutoScroll->setText("NoAutoScroll");
  else
    ui->PitchAutoScroll->setText("AutoScroll");
}
void MainWindow::PitchGraphInspect(bool val){
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
void MainWindow::PitchErrorGraph(int val){
  PITCH_GRAPH_DISPLAY[error]=val;
  if(val==0)ui->graphPitch->graph(error)->data()->clear();
}
void MainWindow::PitchSetpointGraph(int val){
  PITCH_GRAPH_DISPLAY[setPoint]=val;
  if(val==0)ui->graphPitch->graph(setPoint)->data()->clear();
}
void MainWindow::PitchCorrectionGraph(int val){
  PITCH_GRAPH_DISPLAY[correction]=val;
  if(val==0)ui->graphPitch->graph(correction)->data()->clear();
}
void MainWindow::PitchBufferChanged(int val){
  ui->PitchBufferValue->setText(QString::number(val));
  BUFFER=val;
}
void MainWindow::PitchDisplayChanged(int val){
  DISPLAY_TIME=val;
  ui->PitchManualValue->setText(QString::number(DISPLAY_TIME));
}
void MainWindow::setupGraph(QCustomPlot* graph){
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
  graph->yAxis->setLabel("degrees (θ)");
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
void MainWindow::clearGraph(QCustomPlot* plt){
  plt->graph(state)->data()->clear();
  plt->graph(setPoint)->data()->clear();
  plt->graph(error)->data()->clear();
  plt->graph(correction)->data()->clear();
  t=0;
}
void MainWindow::loop(){
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
