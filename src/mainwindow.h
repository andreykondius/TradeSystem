#pragma once

#include <QMainWindow>
#include "trading/tcpserver.h"
#include "tests/test.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    int numTRansact = 0;
    double testProfit = 0.0;
    double testOldProfit = 0.0;
    std::unique_ptr<TcpServer> serv;
    test::Test test;

    void initServ();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *getUi() const;    

private slots:
    void on_startBut_clicked();

    void on_calcdBut_clicked();

    void on_calcdBut_2_clicked();

    void onRes(const double err, const double res, int epoch, bool valid);

    void on_saveCheckBox_stateChanged(int arg1);

    void on_trainBut_clicked();

    void on_trainBut_2_clicked();

    void on_trainingNetBut_clicked();

    void on_initNeuroBut_clicked();

    void onProfit(const double newProf, const char* date);

    void on_stopBut_clicked();

signals:
    void toStopTest(const bool flag);
    void toRes(double err, double res, int epoch, bool valid);

private:
    Ui::MainWindow *ui;
};

