// cell для рекуррентной сети lstmn

#pragma once
#include <vector>
#include <string>

class Neuron;
using VecNeuronPtr = std::vector<Neuron*>;
class Cell
{
    unsigned int countInput;
    Neuron *output;
    VecNeuronPtr arrNeuron;

    bool initialization();
    void deInitialization();
public:
    Cell(const unsigned int countInput);
    virtual ~Cell();
};

