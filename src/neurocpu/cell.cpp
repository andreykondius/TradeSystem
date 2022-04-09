#include "cell.h"
#include "neuron.h"

Cell::Cell(const unsigned int countInput)
{
    initialization();
}

Cell::~Cell()
{
    deInitialization();
}

void Cell::deInitialization()
{
    while(arrNeuron.size() > 0)
    {
        delete arrNeuron[0];
        arrNeuron[0] = 0;
        arrNeuron.erase(arrNeuron.begin());
    }
    arrNeuron.clear();
}

bool Cell::initialization()
{
    if(countInput <= 0)
        return false;
    deInitialization();

    for(unsigned int inp=0; inp < countInput; ++inp)
    {
        arrNeuron.push_back( new Neuron(0, Type::eINPUT) );
    }
}
