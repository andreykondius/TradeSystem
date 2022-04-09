#pragma once
#include "neurocpu/perceptron.h"

#include <memory>
#include <QObject>

namespace test
{

struct Out
{
    double err;
    double output;
    int countEpoch;
    bool validOut;
    Out(){}
    Out(double err, double output, int countEpoch, bool validOut):
        err(err), output(output), countEpoch(countEpoch), validOut(validOut){}
};

class Test: public QObject
{
    Q_OBJECT

    double expected = 0.15;    // for TEST

    std::unique_ptr<Perceptron> nperceptron_TEST;
public:
    Test();

    Out initPerceptron(const int countInput, const int countLayers);
    Out trainingPerceptron(const int countInput, const int countLayers, const double expected, std::vector<double> &arrData);
    Out calcData(std::vector<double> &arrData);

};

}
