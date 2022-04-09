// Нейрон имеет несколько входов и несколько выходов,
// но для удобства отмечается один вход/выход, массив входных весов и массив указателей на связанные с выходом нейроны.
// Входные нейроны всегда имеют один вход, совпадающий с выходом

#pragma once

#include <vector>
#include <memory>
#include <map>

enum Type{
    eINPUT,
    eHIDDEN,
    eBIAS,
    eOUTPUT
};
enum TypeFunc{
    eSigmoid,
    eTanh
};

class Neuron;

using VecNeuronPtr = std::vector<Neuron*>;
using MapNeuronWeight = std::map<Neuron*, double>;
class Neuron
{
    Type type;
    TypeFunc typeFunc;
    int indexLayer;     // номер слоя
    double input;
    double out;
    double outIdeal;    // идеальный Out
    double sum;
    double delta;
    double dError;                  // Ошибка сети в данном узле
    MapNeuronWeight weight;
    VecNeuronPtr pOut;
    void pushRandomWeight(Neuron *neuronInp);
    void funcAcivation();

public:
    Neuron(const int layer, const Type type, const TypeFunc typeFunc=TypeFunc::eTanh);
//    Neuron(const int layer, const Type type, const TypeFunc typeFunc=TypeFunc::eSigmoid);
    ~Neuron();

    double sigmoid(const double x);
    double tanh(const double x);

    void summator();
    void pushLink(Neuron *neuronOut);
    void setInput(const double val);
    double getInput() const;
    double getOut() const;
    double getSum() const;
    VecNeuronPtr &getPOut();
    TypeFunc getTypeFunc() const;
    void setTypeFunc(const TypeFunc val);
    Type getType() const;
    void setType(const Type &val);
    double getWeight(Neuron *val);
    MapNeuronWeight getWeights();
    void setWeight(Neuron *neuron, const double val);
    int getSizeWeight() const;
    double getDError() const;
    void setDError(const double val);
    double getDelta() const;
    void setDelta(const double val);
    int getIndexLayer() const;
};
