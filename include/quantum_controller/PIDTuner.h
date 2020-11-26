#ifndef PIDTUNER_H
#define PIDTUNER_H

#include <QMainWindow>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>
#include <vector>
#include <string>
#include "ui_PIDTuner.h"
#include "quantum_controller/PIDTunerWidget.h"

namespace Ui {
class PIDTuner;
}

class PIDTuner : public QMainWindow
{
    Q_OBJECT

public:
    explicit PIDTuner( QWidget *parent = 0);
    ~PIDTuner();
    void verifyValues(uint16_t [], bool []);
    void updateDataPoints(double [][3], double t);

public slots:
    void TabChanged(int);
    void Load();
    void Save();
    void Edit();
    void Quit();
private:
    void initialiseVariables();
    void setupSlots();
    void loadTabs();
    void addTab(QString);
    void closeEvent(QCloseEvent*);
    int ACTIVE_WINDOW;
    double TIME_OFFSET,T;
    std::string LOGO_PATH,CONFIG_FILE_PATH;
    Ui::PIDTuner *ui;
    std::vector<PIDTunerWidget*> TABS;


};

#endif // PIDTUNER_H
