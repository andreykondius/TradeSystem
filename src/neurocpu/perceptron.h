// Многослойный перцептрон

#pragma once
#include <vector>
#include <string>

class Neuron;
using VecNeuronPtr = std::vector<Neuron*>;
class Perceptron
{
    const double maxError = 0.02;       // максимальная ошибка
    const double eps = 0.15;            // скорость обучения
    const double outExpected = 0.9;    // ожидаемый выход
    const double epsExpected;           // ожидаемая точность сети
    const double epsOutExpected;        // ожидаемая разница между ответом и ожидаемым ответом
    unsigned int countEpoch;            // кол-во эпох
    unsigned int countInput;
    unsigned int countLayers;   // общее кол-во слоёв: 0-входной слой, все послед. слои - скрытые
    Neuron *output;
    VecNeuronPtr arrNeuron;

    double fIn(const Neuron *neuron);
    void backPropagation();
    bool initialization();
    void reInitialization();
    std::string printText(Neuron *neuron, const int indexPOut, const int indexNext, const std::string &text, const bool fOut);

public:
    Perceptron(const int countInput,
             const int countLayers,
             const double outExpected,
             const double epsExpected,
             const double epsOutExpected);     // By default all layers except the last one are the same size. The last layer have a size as 1.
    ~Perceptron();
    void deInitialization();
    void saveToFile(const std::string &activName, const std::string &fileNameWeight, const bool fTextWeights=true);
    void loadFromFile(const std::string &activName, const std::string &fileNameWeight);
    void setINputDataNeuron(const int index, const double val);
    void training();
    void calcData();

    int getCountInput() const;
    int getCountEpoch() const;
    void clearCountEpoch();
    double getError() const;
    double getOutput() const;
    double getEpsExpected() const;
    double getOutExpected() const;

    bool isValid() const;       // достижение требуемой точности
    bool isValidOut() const;    // достижение требуемой разности между расчётным и требуемым ответом сети
    bool isError() const;       // невозможность достижения требуемой точности

};

