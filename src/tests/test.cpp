#include "test.h"
#include <QFile>

namespace test{

Test::Test()
{
}

Out test::Test::initPerceptron(const int countInput, const int countLayers)
{
    this->expected = 1.0;
    Perceptron neuronet(countInput, countLayers, expected, 0.001, 0.001);
    return Out(neuronet.getError(),
               neuronet.getOutput(),
               neuronet.getCountEpoch(),
               neuronet.isValidOut());
}

Out Test::trainingPerceptron(const int countInput, const int countLayers, const double expected, std::vector<double> &arrData)
{
    if(nperceptron_TEST)
        nperceptron_TEST.reset();
    nperceptron_TEST = std::make_unique<Perceptron>(countInput, countLayers, expected, 0.0001, 0.01);

    QFile file("FILES//WEIGHTS//RTS-6.20/weight_test.txt");
    if(file.exists())
        file.remove();
    nperceptron_TEST->loadFromFile("RTS-6.20", "weight_test");
    for(auto i=0; i < nperceptron_TEST->getCountInput(); ++i)
        nperceptron_TEST->setINputDataNeuron(i, arrData[i]);
    for(auto i=0; i < 1000000; ++i)
    {
        nperceptron_TEST->training();
        if(nperceptron_TEST->isValid())
            break;
    }
    nperceptron_TEST->saveToFile("RTS-6.20", "weight_test");
    return Out(nperceptron_TEST->getError(),
               nperceptron_TEST->getOutput(),
               nperceptron_TEST->getCountEpoch(),
               nperceptron_TEST->isValidOut());
}

Out Test::calcData(std::vector<double> &arrData)
{
    if(!nperceptron_TEST)
        return Out();
//    nperceptron_TEST->loadFromFile("RTS-6.20", "weight_test");
    for(auto i=0; i < nperceptron_TEST->getCountInput(); ++i)
        nperceptron_TEST->setINputDataNeuron(i, arrData[i]);
    nperceptron_TEST->calcData();
    return Out(nperceptron_TEST->getError(),
               nperceptron_TEST->getOutput(),
               nperceptron_TEST->getCountEpoch(),
               nperceptron_TEST->isValidOut());
}

}
