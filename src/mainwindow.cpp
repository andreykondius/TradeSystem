#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

double temp[80] = {0.0005878,
                   -1,
                   0,
                   0.0005878,
                   0,
                   0,
                   0,
                   0,
                   -1,
                   0,
                   -2,
                   15,
                   0,
                   0,
                   -16,
                   -1,
                   1,
                   0,
                   -0.036363,
                   0.272727,
                   0,
                   0,
                   -0.290909,
                   -0.018181,
                   -1,
                   0,
                   0,
                   0.0006509,
                   0,
                   0,
                   -0.000694,
                   0,
                   -1,
                   0,
                   0,
                   -2,
                   0,
                   0,
                   0,
                   0,
                   -0.000873,
                   -1,
                   0,
                   -0.001746,
                   0,
                   0,
                   0,
                   0,
                   0,
                   -0.029411,
                   1,
                   -13,
                   -1,
                   1,
                   0,
                   0,
                   1,
                   1,
                   0.0004743,
                   -0.006166,
                   -0.000474,
                   0.0004743,
                   0,
                   0,
                   1,
                   1,
                   0,
                   1,
                   0,
                   0,
                   0,
                   0,
                   0.037037,
                   1,
                   0,
                   0.037037,
                   0,
                   0,
                   0,
                   0
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
}

Ui::MainWindow *MainWindow::getUi() const
{
    return ui;
}


void MainWindow::on_startBut_clicked()      // рестартануть сервер
{
    if(!serv)
    {
        serv = std::make_unique<TcpServer>(2048, ui->saveCheckBox->isChecked());
    }
    serv->setConnect();
}

void MainWindow::on_saveCheckBox_stateChanged(int arg1)  // сохранять/не сохранять данные
{
    if(!serv)
        return;
    serv->setSaveData( ui->saveCheckBox->isChecked() );    // !serv->isSaveData() );
}

void MainWindow::initServ()
{
    if(!serv)
    {
        serv = std::make_unique<TcpServer>(2048, false);
        disconnect(serv.get(), &TcpServer::toRes, this, &MainWindow::onRes);
        connect(serv.get(), &TcpServer::toRes, this, &MainWindow::onRes);
        disconnect(serv.get(), &TcpServer::toProfit, this, &MainWindow::onProfit);
        connect(serv.get(), &TcpServer::toProfit, this, &MainWindow::onProfit);
        disconnect(this, &MainWindow::toStopTest, serv.get(), &TcpServer::onStopTest);
        connect(this, &MainWindow::toStopTest, serv.get(), &TcpServer::onStopTest);
        disconnect(this, &MainWindow::toRes, serv.get(), &TcpServer::toRes);
        connect(this, &MainWindow::toRes, serv.get(), &TcpServer::toRes);
    }
    serv->onStopTest(false);
}

void MainWindow::onRes(const double err, const double res, int epoch, bool valid)
{
    QString textErr = "Ошибка = ";
    textErr.push_back(std::to_string(err).c_str());
    ui->label1->setText( textErr);
    QString textRes = "Результат = ";
    textRes.push_back(std::to_string(res).c_str());
    ui->label2->setText( textRes );
    QString textEpoch = "Эпоха = ";
    textEpoch.push_back(std::to_string(epoch).c_str());
    ui->label3->setText( textEpoch );
    QString textValid = "Ответ = ВЕРНЫЙ ! ";
    if(!valid)
        textValid = "Ответ = !!! НЕ ВЕРНЫЙ !!! ";
    ui->label4->setText( textValid );
}

const double iii = 1.0;
void MainWindow::on_calcdBut_2_clicked()        // посчитать с тренировочными параметрами
{      
    initServ();
    std::vector<double> arrData;
    arrData.emplace_back( ui->lineEdit1->text().toDouble() );
    arrData.emplace_back( ui->lineEdit2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit3->text().toDouble() );
    arrData.emplace_back( ui->lineEdit4->text().toDouble() );
    arrData.emplace_back( ui->lineEdit5->text().toDouble() );
    arrData.emplace_back( ui->lineEdit6->text().toDouble() );
    arrData.emplace_back( ui->lineEdit7->text().toDouble() );
    arrData.emplace_back( ui->lineEdit8->text().toDouble() );
    arrData.emplace_back( ui->lineEdit9->text().toDouble() );
    arrData.emplace_back( ui->lineEdit10->text().toDouble() );
    arrData.emplace_back( ui->lineEdit11->text().toDouble() );
    arrData.emplace_back( ui->lineEdit12->text().toDouble() );
    arrData.emplace_back( ui->lineEdit13->text().toDouble() );
    arrData.emplace_back( ui->lineEdit14->text().toDouble() );
    arrData.emplace_back( ui->lineEdit15->text().toDouble() );
    arrData.emplace_back( 1.0 );

    test::Out par= test.calcData( arrData );
    emit toRes(par.err, par.output, par.countEpoch, par.validOut);
}

void MainWindow::on_trainBut_clicked()
{ 
    initServ();
    std::vector<double> arrData;
    arrData.emplace_back( ui->lineEdit1->text().toDouble() );
    arrData.emplace_back( ui->lineEdit2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit3->text().toDouble() );
    arrData.emplace_back( ui->lineEdit4->text().toDouble() );
    arrData.emplace_back( ui->lineEdit5->text().toDouble() );
    arrData.emplace_back( ui->lineEdit6->text().toDouble() );
    arrData.emplace_back( ui->lineEdit7->text().toDouble() );
    arrData.emplace_back( ui->lineEdit8->text().toDouble() );
    arrData.emplace_back( ui->lineEdit9->text().toDouble() );
    arrData.emplace_back( ui->lineEdit10->text().toDouble() );
    arrData.emplace_back( ui->lineEdit11->text().toDouble() );
    arrData.emplace_back( ui->lineEdit12->text().toDouble() );
    arrData.emplace_back( ui->lineEdit13->text().toDouble() );
    arrData.emplace_back( ui->lineEdit14->text().toDouble() );
    arrData.emplace_back( ui->lineEdit15->text().toDouble() );
    arrData.emplace_back( 1.0 );

//    arrData.clear();
//    for(auto i=0; i < ui->lineEdit1_3->text().toInt(); ++i)
//    {
//        arrData.emplace_back( temp[i] );
//    }

    test::Out par = test.trainingPerceptron( ui->lineEdit1_3->text().toInt(), ui->lineEdit1_4->text().toInt(),
                                   ui->lineEdit1_5->text().toDouble(), arrData );
    emit toRes(par.err, par.output, par.countEpoch, par.validOut);

}

void MainWindow::on_calcdBut_clicked()
{
    initServ();
    std::vector<double> arrData;
    arrData.emplace_back( ui->lineEdit1_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit2_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit3_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit4_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit5_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit6_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit7_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit8_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit9_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit10_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit11_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit12_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit13_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit14_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit15_2->text().toDouble() );
    arrData.emplace_back( 1.0 );

//    arrData.clear();
//    int index = 0;
//    for(auto i=ui->lineEdit1_3->text().toInt()-1; i >= 0; --i)
//    {
//        ++index;
//        if(index > 15)
//            index = 1;
//        arrData.emplace_back( index*2*iii );
//    }

    test::Out par = test.calcData( arrData );
    emit toRes(par.err, par.output, par.countEpoch, par.validOut);
}

void MainWindow::on_trainBut_2_clicked()
{  
    initServ();
    std::vector<double> arrData;
    arrData.emplace_back( ui->lineEdit1_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit2_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit3_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit4_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit5_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit6_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit7_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit8_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit9_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit10_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit11_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit12_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit13_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit14_2->text().toDouble() );
    arrData.emplace_back( ui->lineEdit15_2->text().toDouble() );
    arrData.emplace_back( 1.0 );

//    arrData.clear();
//    int index = 0;
//    for(auto i=ui->lineEdit1_3->text().toInt()-1; i >= 0; --i)
//    {
//        ++index;
//        if(index > 15)
//            index = 1;
//        arrData.emplace_back( index*2*iii );
//    }

    test::Out par = test.trainingPerceptron( ui->lineEdit1_3->text().toInt(), ui->lineEdit1_4->text().toInt(), ui->lineEdit1_5->text().toDouble(), arrData );
    emit toRes(par.err, par.output, par.countEpoch, par.validOut);
}

void MainWindow::on_trainingNetBut_clicked()
{
    initServ();        
    numTRansact = 0;
    for(auto i=0; i < 100000; ++i)
    {
        testOldProfit = testProfit;
        numTRansact = i;
        testProfit = 0.0;
        serv->trainingNetFromData( QDir("FILES\\HISTORY"), numTRansact );
    }
}

void MainWindow::on_initNeuroBut_clicked()
{
    initServ();
    const int countInput = ui->lineEdit1_3->text().toInt();
    const int countLayers = ui->lineEdit1_4->text().toInt();
    test::Out par = test.initPerceptron(countInput, countLayers);
    emit toRes(par.err, par.output, par.countEpoch, par.validOut);
}

void MainWindow::onProfit(const double newProf, const char* date)
{
    QString textOld = "Общий профит = " + QString(std::to_string(testOldProfit).c_str());
    ui->labelOldProfit->setText( textOld );

    QString textNew = "Профит = " + QString(std::to_string(newProf).c_str());
    ui->labelNewProfit->setText( textNew);

    testProfit += newProf;
    QString textSum = "Итоговый профит = " + QString(std::to_string(testProfit).c_str());
    ui->labelSumProfit->setText( textSum);

    QString textDate = "Дата обработки = " + QString(date);
    ui->labelCurDate->setText( textDate );

    QString textNum = "Номер транзакции = " + QString( std::to_string(numTRansact).c_str() );
    ui->labelTransact->setText( textNum );
}

void MainWindow::on_stopBut_clicked()
{
    emit toStopTest(true);
}
