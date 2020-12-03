#include "quantum_controller/PIDTuner.h"

PIDTuner::PIDTuner( QWidget *parent) :
    QMainWindow(parent), ui(new Ui::PIDTuner)
{
    initialiseVariables();
    setupSlots();
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
void PIDTuner::Load(){
  for(PIDTunerWidget *tab:TABS){
    tab->Load();
  }
}
void PIDTuner::Save(){
  for(PIDTunerWidget *tab:TABS){
    tab->Save();
  }
}
void PIDTuner::Edit(){
  system(("gedit "+CONFIG_FILE_PATH).c_str());
  TabChanged(ACTIVE_WINDOW);
}
void PIDTuner::Quit(){
  this->close();
}
void PIDTuner::initialiseVariables(){
  LOGO_PATH = ros::package::getPath("quantum_controller") + "/utils/logo.png";
  CONFIG_FILE_PATH=ros::package::getPath("quantum_controller")+"/config/config.pid";
  TIME_OFFSET=T=0;
  ACTIVE_WINDOW=0;
  ui->setupUi(this);
  QPixmap pm(QString::fromStdString(LOGO_PATH));
  ui->logo->setPixmap(pm.scaled(ui->logo->size()));
  ui->logo->setScaledContents(true);
}
void PIDTuner::setupSlots(){
  connect(ui->Tuning,SIGNAL(currentChanged(int)),this,SLOT(TabChanged(int)));
  connect(ui->action_Load,SIGNAL(triggered()),this,SLOT(Load()));
  connect(ui->action_Save,SIGNAL(triggered()),this,SLOT(Save()));
  connect(ui->action_Edit,SIGNAL(triggered()),this,SLOT(Edit()));
  connect(ui->action_Quit,SIGNAL(triggered()),this,SLOT(Quit()));
}
void PIDTuner::addTab(QString label){
  TABS.push_back(new PIDTunerWidget(label));
  ui->Tuning->addTab(TABS.back(),label);
}
void PIDTuner::loadTabs(){
  std::string line;
  std::vector <std::string> tokens;
  std::fstream f;
  f.open(CONFIG_FILE_PATH,std::ios::in);
  while (std::getline(f, line))
  {
     boost::trim(line);
     if (line[0] == '#' || line.size() == 0)
       continue;
     boost::split(tokens,line,boost::is_any_of(",:|[]()<>"));
     addTab(QString::fromStdString(tokens[0]));
  }
  f.close();
}
void PIDTuner::verifyValues(uint16_t val[], bool arr[]){

}
void PIDTuner::updateDataPoints(double val[][3], double t){
  T=t;
  TABS.at(ACTIVE_WINDOW)->updateDataPoints(val[ACTIVE_WINDOW],t-TIME_OFFSET);
}
void PIDTuner::closeEvent (QCloseEvent *event){
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "PID Tuner",
                      "<FONT COLOR='#BDBDBD'>Do you really want to Quit?</FONT>",
                      QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                      QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}
