#ifndef PIDTUNER_H
#define PIDTUNER_H

#include <QMainWindow>
#include <QApplication>
#include <QPixmap>
// #include <stdio.h>
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
    void updateDataPoints(double [][4], double t);

public slots:
    void TabChanged(int);

private:
    void setup();
    void loadTabs();
    void addTab(QString);
    int ACTIVE_WINDOW;
    double TIME_OFFSET,T;
    std::string LOGO_PATH,CONFIG_PATH;
    Ui::PIDTuner *ui;
    std::vector<PIDTunerWidget*> TABS;


};

#endif // PIDTUNER_H
