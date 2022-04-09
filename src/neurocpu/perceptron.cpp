#include "perceptron.h"
#include "neuron.h"

#include <fstream>
#include <sstream>
#include <QDir>

Perceptron::Perceptron(const int countInput,
                   const int countLayers,
                   const double outExpected,
                   const double epsExpected,
                   const double epsOutExpected)
    : outExpected(outExpected), epsExpected(epsExpected), epsOutExpected(epsOutExpected), countEpoch(0), countInput(countInput), countLayers(countLayers)
{
    initialization();
}

Perceptron::~Perceptron()
{
    deInitialization();
}

void Perceptron::deInitialization()
{
    while(arrNeuron.size() > 0)
    {
        delete arrNeuron[0];
        arrNeuron[0] = 0;
        arrNeuron.erase(arrNeuron.begin());
    }
    arrNeuron.clear();
}

bool Perceptron::initialization()
{
    if(countInput <= 0)
        return false;
    deInitialization();

    for(unsigned int inp=0; inp < countInput; ++inp)
    {
        arrNeuron.push_back( new Neuron(0, Type::eINPUT) );
    }
    arrNeuron.push_back( new Neuron(0, Type::eBIAS) );
    arrNeuron[ arrNeuron.size()-1 ]->setInput( 1.0 );
    int indexStart = 0;
    for(unsigned int l=1; l < countLayers; ++l)
    {
        VecNeuronPtr arrTempNeuron;
        for(unsigned int i=0; i < countInput+1; ++i)
        {
            Neuron *neuron = new Neuron( l, Type::eHIDDEN );
            arrTempNeuron.push_back( neuron );
            if(i == countInput)
            {
                arrTempNeuron[ arrTempNeuron.size()-1 ]->setType( Type::eBIAS );
                arrTempNeuron[ arrTempNeuron.size()-1 ]->setInput( 1.0 );
                continue;
            }
            for(unsigned int j=indexStart; j < arrNeuron.size(); ++j)
            {
                arrNeuron[j]->pushLink( neuron );
            }
        }
        indexStart = arrNeuron.size();
        for(unsigned int i=0; i < countInput+1; ++i)
            arrNeuron.push_back( arrTempNeuron[i] );
    }

    output = new Neuron( countLayers-1, Type::eOUTPUT );
    for(int i = arrNeuron.size()-1; i >= indexStart; --i)
        arrNeuron[i]->pushLink( output );
    arrNeuron.push_back( output );

    return true;
}

void Perceptron::reInitialization()
{
    deInitialization();
    std::vector<double> arrInp;
    int index = -1;
    for(auto it : arrNeuron)
    {
        ++index;
        if(index > countInput-1)
            break;
        arrInp.push_back( it->getInput() );
    }
    initialization();
    for(auto i=0; i < countInput; ++i)
        arrNeuron[i]->setInput(arrInp[i]);
}

void Perceptron::training()       // натренировать сеть
{
    ++countEpoch;
    calcData();
    backPropagation();
    calcData();
}

void Perceptron::setINputDataNeuron(const int index, const double val)
{
    if(index < 0 || index > arrNeuron.size()-1)
        return;

    if(arrNeuron[index]->getType() == Type::eINPUT)
    {
        double delta = val;
        if(delta >= 100.0)
            delta = 100.0;
        if(delta <= -100.0)
            delta = -100.0;
//        arrNeuron[index]->setInput( 1.0-exp(-delta) );
        arrNeuron[index]->setInput( delta );
    }
}

double Perceptron::fIn(const Neuron *neuron)
{
    double curOut = neuron->getOut();
    double result = 0.0;
    if(neuron->getTypeFunc() == TypeFunc::eSigmoid)
        result = (1.0-curOut)*curOut;
    else
        if(neuron->getTypeFunc() == TypeFunc::eTanh)
            result = 1.0 - curOut*curOut;
    return result;
}

void Perceptron::backPropagation()
{
    int index = arrNeuron.size();
    for(;;)
    {
        --index;
        if(index < 0)
            break;
        Neuron *neuron = arrNeuron[index];
        if(neuron == nullptr)
            continue;
        if(neuron->getType() == Type::eOUTPUT)
            continue;
        double sum = 0.0;
        for(auto p : neuron->getPOut())
        {
            sum += p->getWeight( neuron ) * p->getDelta();
        }
        neuron->setDelta( sum );
        for(auto &p : neuron->getPOut())
        {
            double delta = sum * fIn(p);
            double gradW = delta * neuron->getOut();
            double deltaWeight = eps * gradW;
            p->setWeight( neuron, p->getWeight(neuron)+deltaWeight );
        }
    }
}

void Perceptron::calcData()   // проверить результат
{
    for(auto it : arrNeuron)
    {
        it->summator();
    }

    double tempOut = (output->getOut());
    double tempOutExpected = (outExpected);
    if(tempOut < 0.0)
    {
        tempOutExpected = -tempOutExpected;
    }
    double delta  = (tempOutExpected - tempOut) * fIn(output);
    output->setDelta( delta );
    double dError = ( (tempOutExpected - tempOut) * (tempOutExpected - tempOut) );
    output->setDError( dError );

}

int Perceptron::getCountInput() const
{
    return countInput;
}

int Perceptron::getCountEpoch() const
{
    return countEpoch;
}

void Perceptron::clearCountEpoch()
{
    countEpoch = 0;
}

double Perceptron::getError() const
{
    if(output == nullptr)
        return 111.0;
    return output->getDError();
}

double Perceptron::getOutput() const
{
    if(output == nullptr)
        return 111.0;
    return output->getOut();
}

double Perceptron::getEpsExpected() const
{
    return epsExpected;
}

double Perceptron::getOutExpected() const
{
    return outExpected;
}

bool Perceptron::isValid() const
{
    return std::abs(output->getDError()) <= epsExpected;
}

bool Perceptron::isValidOut() const
{
    bool res = std::abs(std::abs(output->getOut()) - std::abs(outExpected)) <= epsOutExpected;
    return res;
}

bool Perceptron::isError() const
{
    return output->getDError() > maxError;
}

std::string toString(const double val)
{
    std::stringstream str;
    str.precision(15);
    str << val;
    return str.str();
}


std::string Perceptron::printText(Neuron *neuron, const int indexPOut, const int indexNext, const std::string &text, const bool fOut)
{
    if(indexPOut >= neuron->getPOut().size())
        return text;

    Neuron *next = nullptr;
    std::string res;
    if(neuron->getType() == Type::eBIAS)
    {
        int index = -1;
        for(auto i=0; i < arrNeuron.size(); ++i)    // поиск след bias
        {
            if(arrNeuron[i] == neuron)
                index = i;
            if(index == -1)
                continue;
            if(index != i && arrNeuron[i]->getType() == Type::eBIAS)
            {
                next = arrNeuron[i];
                break;
            }
        }
    }
    else
    {
        if(indexNext < neuron->getPOut().size())
            next = neuron->getPOut()[indexNext];
    }
    Neuron *newNeuron = neuron->getPOut()[indexPOut];
    if(!newNeuron)
        return text;

    res += "   W = " + std::to_string(newNeuron->getWeight(neuron));
    if(fOut)
        res += "    Out = " + std::to_string(newNeuron->getOut()) + "  ";// +"/" + std::to_string(newNeuron->getDelta()) + "  ";
    else
        res += "                    ";
    if(!next)
        return text+res;
    res = printText(next, indexPOut, indexNext, text + res, fOut);
    return res;
}

void Perceptron::saveToFile(const std::string &activName, const std::string &fileNameWeight, const bool fTextWeights)
{
    QDir dir;
    dir.mkpath( "FILES\\WEIGHTS\\"+QString(activName.c_str()) );
if(1 == 2)
    {
        std::string fileName = "FILES\\WEIGHTS\\"+activName+"\\" + fileNameWeight + ".bin";
        std::ofstream file;
        file.open(fileName, std::ios_base::out | std::ios_base::binary);

        file.write(std::to_string(countInput).c_str(), 4);
        file.write(std::to_string(countLayers).c_str(), 4);

        for(unsigned int i=0; i < arrNeuron.size(); ++i)
        {
            for(auto w : arrNeuron[i]->getWeights())
            {
                double weight = w.second;
                file.write(toString(weight).c_str(), 30);
            }
        }
        file.close();
    }

    if(fTextWeights)
    {
        std::string fileNameTxt = "FILES\\WEIGHTS\\"+activName+"\\"+fileNameWeight+".txt";
        std::ofstream textFile(fileNameTxt, std::ios_base::app);

//        printText( textFile, arrNeuron[ arrNeuron.size()-1 ] );

        int indexPOut = -1;
        for(;;)
        {
            ++indexPOut;
            bool fExit = false;
            bool fOut = true;
            for(auto i=0; i < countInput+1; ++i)
            {
                if(indexPOut >= arrNeuron[i]->getPOut().size())
                {
                    fExit = true;
                    break;
                }
                if(arrNeuron[i]->getType() == Type::eINPUT )
                    textFile << "Inp = " << arrNeuron[i]->getOut() << "     ";
                if(arrNeuron[i]->getType() == Type::eBIAS )
                    textFile << "Bias = " << (int)arrNeuron[i]->getOut() << "    ";
                std::string text = printText(arrNeuron[i], indexPOut, i, "", fOut);
                textFile << text << std::endl;
                fOut = false;
            }
            if(fExit)
                break;
        }
        textFile << "Epoch = " << countEpoch << ", dError = " << output->getDError() << std::endl;
        textFile << "_____________________________________________________________________________" <<std::endl;
        textFile.close();
    }
}

void Perceptron::loadFromFile(const std::string &activName, const std::string &fileNameWeight)
{
    std::string fileName = "FILES\\WEIGHTS\\" + activName + "\\" + fileNameWeight + ".bin";
    std::fstream file;
    file.open(fileName, std::ios_base::in | std::ios_base::binary);
    if(!file.is_open())
        return;

    char cSz[4];
    file.read(cSz, 4);
    countInput = std::stoi(cSz);

    file.read(cSz, 4);
    countLayers = std::stoi(cSz);

    initialization();

    for(unsigned int i=0; i < arrNeuron.size(); ++i)
    {
        for(auto w : arrNeuron[i]->getWeights())
        {
            char cSzDouble[30];
            file.read(cSzDouble, 30);
            arrNeuron[i]->setWeight(w.first, std::stod(cSzDouble));
        }
    }
    file.close();
}
