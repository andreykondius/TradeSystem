#include "neuron.h"
#include <cmath>
#include <random>
#include <chrono>
#include <ctime>

#include <iostream>

Neuron::Neuron(const int layer, const Type type, const TypeFunc typeFunc)
    : type(type), typeFunc(typeFunc), indexLayer(layer), input(0.0), out(0.0), outIdeal(0.0)
{
}

Neuron::~Neuron()
{
    pOut.clear();
    weight.clear();
}

double Neuron::sigmoid(const double x)
{
    double temp = x;
    if(temp >= 100.0)
        temp = 100.0;
    return 1.0/(1.0+exp(-temp));
}

double Neuron::tanh(const double x)
{
    double temp = 2.0*x;
    if(temp >= 100.0)
        temp = 100.0;
    return (exp(temp)-1.0)/(exp(temp)+1.0);
}

void Neuron::pushLink(Neuron *neuronOut)
{
    if(neuronOut)
    {
        pOut.push_back( neuronOut );
        neuronOut->pushRandomWeight( this );
    }
}

void Neuron::setInput(const double val)
{
    input = val;
    if(type == Type::eINPUT || type == Type::eBIAS)
        out = val;
}

double Neuron::getInput() const
{
    return input;
}

void Neuron::pushRandomWeight(Neuron* neuronInp)
{
    if(!neuronInp)
        return;
    static int index = 0;
    ++index;
    if(index > INT_MAX)
        index = 1;
    std::mt19937 gen( time(0)+index );
//    std::uniform_real_distribution<> el(0.0001, 0.001);
//    std::uniform_real_distribution<> el(0.001, 0.01);
//    std::uniform_real_distribution<> el(0.1, 1.0);
    std::uniform_real_distribution<> el(0.1, 1.0);

    weight[ neuronInp ] = el(gen)* 1e-1;
}

double Neuron::getOut() const
{
    return out;
}

double Neuron::getSum() const
{
    return sum;
}

VecNeuronPtr &Neuron::getPOut()
{
    return pOut;
}

TypeFunc Neuron::getTypeFunc() const
{
    return typeFunc;
}

void Neuron::setTypeFunc(const TypeFunc val)
{
    typeFunc = val;
}

Type Neuron::getType() const
{
    return type;
}

void Neuron::setType(const Type &val)
{
    type = val;
}

double Neuron::getWeight(Neuron *val)
{
    return weight[val];
}

MapNeuronWeight Neuron::getWeights()
{
    return weight;
}

void Neuron::setWeight(Neuron *neuron, const double val)
{
    weight[neuron] = val;
}

int Neuron::getSizeWeight() const
{
    return weight.size();
}

void Neuron::summator()     // the forward propagation
{
    if(type == Type::eINPUT || type == Type::eBIAS)
        return;
    double tempOut = 0.0;
    for(auto w: weight)
    {
        tempOut += w.first->getOut() * w.second;
    }
    sum = tempOut;
    funcAcivation();
}

void Neuron::funcAcivation()
{
    if(typeFunc == TypeFunc::eTanh)
        out = tanh(sum);
    if(typeFunc == TypeFunc::eSigmoid)
        out = sigmoid(sum);
}

double Neuron::getDError() const
{
    return dError;
}

void Neuron::setDError(const double val)
{
    dError = val;
}

double Neuron::getDelta() const
{
    return delta;
}

void Neuron::setDelta(const double val)
{
    delta = val;
}

int Neuron::getIndexLayer() const
{
    return indexLayer;
}
