#pragma once

#include "neurocpu/neuron.h"
#include "neurocpu/perceptron.h"
#include <memory>
#include <QObject>
#include <vector>

using llint = long long int;

const int sizeArrTradeData = 8;
#pragma pack(push, 1)
struct TradeData    // 8
{
    double deltaBid = 0.0;
    double deltaAsk = 0.0;
    double deltaLast = 0.0;
    double deltaVolume = 0.0;
    double deltaBuy = 0.0;
    double volumeBuy = 0.0;
    double volumeSell = 0.0;
    double deltaSell = 0.0;
    double sum()
    {
        return deltaBid + deltaAsk + deltaLast + deltaVolume + deltaBuy + volumeBuy + volumeSell + deltaSell;
    }
};

enum TradeCondition
{
    eNONE_TRADE,
    eBUY_TRADE,
    eSELL_TRADE,
    eBUYCLOSE_TRADE,
    eSELLCLOSE_TRADE
};

const int sizeTradeData = sizeof(TradeData) / sizeof(double);
const int sizeArrayTrade = sizeTradeData * 10;
using ArrayTradeData = double[sizeArrayTrade];

class TradeManager// : public QObject
{
    const std::string nameWeightBuy          = "weightbuy";
    const std::string nameWeightSell         = "weightsell";

    const int countInput = 80;
    const int countLayers = 2;
    const int countEpoch = 10;             // количество эпох для обучения

    const int minTimeDeal = 1000;          // min время сделки
    const double stopLoss = -20.0;
    const double profit = 40.0;            // минимальный профит
    const double profitTraining = 100.0;   // минимальный профит для обучения

    const double minSumInput = 50.0;
    const double stepPrice;                 // шаг цены
    const double eps = 1e-3;                // точность ответа нейросети
    const double epsTrade = 1e-3;           // точность принятия нейросетью решения о заходе в сделку

    const bool fTest = false;
    TradeCondition condition = eNONE_TRADE;
    std::string activName;
    std::string curTestDataTime;

    int indexArrayTradeData = 0;
    double testProfit = 0.0;

    struct Deal{
        std::string dateTime;
        llint time;
        double price = 0.0;
    };

    Deal buy;
    Deal sell;
    Deal buyTraining;
    Deal buyTrainingOld;
    Deal sellTraining;
    Deal sellTrainingOld;

    double bidOld;
    double askOld;
    double lastOld;
    double volumeOld;
    double buyOld;
    double sellOld;
    llint timeBid;
    llint timeAsk;
    llint timeLast;
    llint timeVolume;
    llint timeBuy;
    llint timeSell;

    TradeData trade = {0.0};
    ArrayTradeData arrTrade = {0.0};

    ArrayTradeData saveBuy = {0.0};
    ArrayTradeData saveSell = {0.0};
    ArrayTradeData saveBuyTrade = {0.0};
    ArrayTradeData saveSellTrade = {0.0};

    std::unique_ptr<Perceptron> netBuy;
    std::unique_ptr<Perceptron> netSell;

    void initNeuroNet(std::unique_ptr<Perceptron> &net, const ArrayTradeData &trade);
    double sumTradeData();


public:
    TradeManager(const std::string &activName, const double stepPrice, const int indexEpoch, const bool fTest);
    ~TradeManager();
    void calcData(const char *c);

    void saveToWeights(const bool fTextWeight);
    void loadFromWeights();
    TradeCondition getCondition() const;
    void setCondition(const TradeCondition &val);
    double getTestProfit() const;
    std::string getCurDate() const;
};
