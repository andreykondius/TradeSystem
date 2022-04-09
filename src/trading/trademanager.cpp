#include "trademanager.h"
#include "database/data.h"
#include "logrec/putlog.hpp"

#undef slots
#include <torch/torch.h>
#define slots Q_SLOTS

#include <cmath>
#include <iostream>

TradeManager::TradeManager(const std::string &activName, const double stepPrice, const int indexEpoch, const bool fTest)
    : stepPrice(stepPrice),
      fTest(fTest), activName(activName), testProfit(0.0),
      bidOld(0.0), askOld(0.0), lastOld(0.0), volumeOld(0.0), buyOld(0.0), sellOld(0.0),
      timeAsk(0.0),  timeLast(0.0),  timeVolume(0.0), timeBuy(0.0), timeSell(0.0)
{
    SingltoneCollection::instance().createLogFile();
    Putlog(LOG_INFO, "------------------------------", indexEpoch, "-------------------------------------------------------------------");

    netBuy          = std::make_unique<Perceptron>( countInput, countLayers, 0.5, eps, epsTrade );
    netBuy->loadFromFile(activName, nameWeightBuy);
    netSell         = std::make_unique<Perceptron>( countInput, countLayers, 0.5, eps, epsTrade );
    netSell->loadFromFile(activName, nameWeightSell);

//    at::Tensor tensor = torch::rand({2, 3});
//    std::cout << tensor << std::endl;
}

TradeManager::~TradeManager()
{
    Putlog(LOG_INFO, "=================================================================================================");
}

void TradeManager::initNeuroNet(std::unique_ptr<Perceptron> &net, const ArrayTradeData &trade)
{
    net->clearCountEpoch();
    for(auto i=0; i < sizeArrayTrade; ++i)
        net->setINputDataNeuron(i, trade[i]);
}

double TradeManager::sumTradeData()
{
    double res = 0.0;
    for(auto it: arrTrade)
        res += std::abs(it);
    return res;
}

void TradeManager::calcData(const char *c)
{
    Data data( c );
    int flags = data.getFlags();
    llint time = data.getTime();
    double bid = data.getBid();
    double ask = data.getAsk();
    double last = data.getLast();
    double volume = data.getVolume_Real();    

    if(ask <= 0.0 || bid <= 0.0 || last <= 0.0 || volume <= 0.0)
        return;

    if(curTestDataTime.size() == 0)
        curTestDataTime = data.getDateTime();
    trade.deltaBid /= double(time-timeBid+1);
    trade.deltaAsk /= double(time-timeAsk+1);
    trade.deltaLast /= double(time-timeLast+1);
    trade.deltaVolume /= double(time-timeVolume+1);
    trade.deltaBuy /= double(time-timeBuy+1);
    trade.deltaSell /= double(time-timeSell+1);
    trade.volumeBuy /= double(time-timeBuy+1);
    trade.volumeSell /= double(time-timeSell+1);

    if(flags & 0x02)        // TICK_FLAG_BID
    {
        if(bidOld > 0.0 && bidOld != bid)
        {
            trade.deltaBid = (bid - bidOld) / stepPrice;
        }
        timeBid = time;
        bidOld = bid;
    }

    if(flags & 0x04)        // TICK_FLAG_ASK
    {
        if(askOld > 0.0 && askOld != ask)
        {
            trade.deltaAsk = (ask - askOld) / stepPrice;
        }
        timeAsk = time;
        askOld = ask;
    }

    if(flags & 0x08)        // TICK_FLAG_LAST
    {
        if(lastOld > 0.0 && lastOld != last)
        {
           trade.deltaLast = (last - lastOld) / stepPrice;
        }
        timeLast = time;
        lastOld = last;
    }

    if(flags & 0x10)        // TICK_FLAG_VOLUME
    {
        if(volumeOld > 0.0 && volumeOld != volume)
        {
            trade.deltaVolume = volume - volumeOld;
        }
        timeVolume = time;
        volumeOld = volume;
    }

    if(flags & 0x20)        // TICK_FLAG_BUY
    {
        if(buyOld > 0.0 && buyOld != last)
        {
            trade.deltaBuy = (last - buyOld) / stepPrice;
            trade.volumeBuy = volume;
        }
        timeBuy = time;
        buyOld = last;
    }

    if(flags & 0x40)        // TICK_FLAG_SELL
    {
        if(sellOld > 0.0 && sellOld != last)
        {
            trade.deltaSell = (last - sellOld) / stepPrice;
            trade.volumeSell = -volume;
        }
        timeSell = time;
        sellOld = last;
    }

//    arrTrade[indexArrayTradeData] = trade;
    memcpy(&arrTrade[indexArrayTradeData], &trade, sizeTradeData*sizeof(double));
    indexArrayTradeData += sizeTradeData;
    if(indexArrayTradeData == sizeArrayTrade)
        indexArrayTradeData = 0;


    auto buyTrainingStart = [&]()
    {
        initNeuroNet(netBuy, saveBuy);
        Putlog(LOG_INFO, "The TRAINING to BUY on price = ", buyTraining.price, "/", bid,
               ", datetTime = ", buyTraining.dateTime, "/", data.getDateTime());
        for(auto i=0; i < countEpoch; ++i)
        {
            netBuy->training();
            if(netBuy->isValid())
                break;
            //                if(netBuy->isValid())
            //                    netBuy->saveNetwork();
            //                else
            //                    netBuy->comeBackToSave();
        }
    };
    auto sellTrainingStart = [&]()
    {
        initNeuroNet(netSell, saveSell);
        Putlog(LOG_INFO, "The TRAINING to SELL on price = ",sellTraining.price, "/", bid,
               ", datetTime = ", sellTraining.dateTime, "/", data.getDateTime());
        for(auto i=0; i < countEpoch; ++i)
        {
            netSell->training();
            if(netSell->isValid())
                break;
        }
    };
    auto closeBuy = [&](const double profit)   // ----------------
    {
        //                emit toBuyClose();
        if(fTest)
        {
            if(profit > 0)
            {
                buyTraining.price = buy.price;
                buyTraining.dateTime = buy.dateTime;
                memcpy(&saveBuy, &saveBuyTrade, sizeArrayTrade*sizeof(double));
//                buyTrainingStart();
            }
            condition = TradeCondition::eNONE_TRADE;
            testProfit += profit;
            Putlog(LOG_INFO, "The BUY closed at the price = ", buy.price, "/", bid, ", profit = ", profit,
                   ", sumProfit = ", testProfit, ", BUY dateTime = ", buy.dateTime , ", CLOSE buy dateTime = ", data.getDateTime());
        }
    };
    auto closeSell = [&](const double profit)
    {
        //                emit toSellClose();
        if(fTest)
        {
            if(profit > 0)
            {
                sellTraining.price = sell.price;
                sellTraining.dateTime = sell.dateTime;
                memcpy(&saveSell, &saveSellTrade, sizeArrayTrade*sizeof(double));
//                sellTrainingStart();
            }
            condition = TradeCondition::eNONE_TRADE;
            testProfit += profit;
            Putlog(LOG_INFO, "The SELL closed at the price = ", sell.price, "/", ask, ", profit = ", profit,
                   ", sumProfit = ", testProfit, ", SELL dateTime = ", sell.dateTime , ", CLOSE sell dateTime = ", data.getDateTime());
        }
    }; // ==================================

    if(condition == TradeCondition::eBUY_TRADE)
    {
        if(   (ask - buy.price <= stopLoss && time - buy.time >= minTimeDeal)
           || (bid - buy.price >= profit && ask < buyTrainingOld.price))
        {
            double profit = bid - buy.price;
            closeBuy(profit);
        }
    }
    if(condition == TradeCondition::eSELL_TRADE)
    {
        if(    (sell.price - bid <= stopLoss && time - sell.time >= minTimeDeal)
            || (sell.price - ask >= profit && bid > sellTrainingOld.price) )
        {
            double profit = sell.price - ask;
            closeSell(profit);
        }
    }

    // ---------- the process of training a neuro net --------------------
    if(buyTraining.price != 0.0 && bid - buyTrainingOld.price < 0.0)    // BUY
    {
        double buyProfit = bid - buyTraining.price;
        if(buyProfit >= profitTraining)// && time == 1587127457073)
            buyTrainingStart();
        buyTraining.price = 0.0;
    }
    if(buyTraining.price == 0.0)
    {
        buyTraining.price = ask;
        buyTraining.dateTime = data.getDateTime();
        memcpy(&saveBuy, &arrTrade, sizeArrayTrade*sizeof(double));
    }

    if(sellTraining.price != 0.0 && sellTrainingOld.price-ask < 0.0)    // SELL
    {
        double sellProfit = sellTraining.price - ask;
        if(sellProfit >= profitTraining)
            sellTrainingStart();
        sellTraining.price = 0.0;
    }
    if(sellTraining.price == 0.0)
    {
        sellTraining.price = bid;
        sellTraining.dateTime = data.getDateTime();
        memcpy(&saveSell, &arrTrade, sizeArrayTrade*sizeof(double));
    }

    if( sumTradeData() >= minSumInput )
    {
        if(condition != TradeCondition::eBUY_TRADE)    // BUY
        {
            initNeuroNet(netBuy, arrTrade);
            netBuy->calcData();
            if(netBuy->isValid() && netBuy->isValidOut())    // вход в сделку
            {
                //                netBuy->saveToFile(activName, "22222222222222.txt");
                //                emit toBuy();
                if(fTest && time - sell.time >= minTimeDeal)
                {
                    memcpy(&saveBuyTrade, &arrTrade, sizeArrayTrade*sizeof(double));
                    buy.price = ask;
                    buy.dateTime = data.getDateTime();
                    buy.time = time;
                    Putlog(LOG_INFO, "The BUY on price = ", buy.price, ", dateTime = ", buy.dateTime);
                    if(condition == TradeCondition::eSELL_TRADE)
                        closeSell(sell.price - buy.price);
                    else
                        condition = TradeCondition::eBUY_TRADE;
                }
            }
        }

        if(condition != TradeCondition::eSELL_TRADE)    //SELL
        {
            initNeuroNet(netSell, arrTrade);
            netSell->calcData();
            if(netSell->isValid() && netSell->isValidOut())       // вход в сделку
            {
                //                emit toSell();
                if(fTest && time - buy.time >= minTimeDeal)
                {
                    memcpy(&saveSellTrade, &arrTrade, sizeArrayTrade*sizeof(double));
                    sell.price = bid;
                    sell.dateTime = data.getDateTime();
                    sell.time = time;
                    Putlog(LOG_INFO, "The SELL on price = ", sell.price, ", dateTime = ", sell.dateTime);
                    if(condition == TradeCondition::eBUY_TRADE)
                        closeBuy(sell.price - buy.price);
                    else
                        condition = TradeCondition::eSELL_TRADE;
                }
            }
        }
    }
    // ======================================================================== */
    buyTrainingOld.price = ask;
    sellTrainingOld.price = bid;

    if(data.getData())
        calcData( data.getData() );
}

void TradeManager::saveToWeights(const bool fTextWeight)
{
//    if(netBuy && netBuy->isValid())
        netBuy->saveToFile(activName, nameWeightBuy, fTextWeight);
//    if(netSell && netSell->isValid())
        netSell->saveToFile(activName, nameWeightSell, fTextWeight);
}

void TradeManager::loadFromWeights()
{
    if(netBuy)
        netBuy->loadFromFile(activName, nameWeightBuy);
    if(netSell)
        netSell->loadFromFile(activName, nameWeightSell);
}

TradeCondition TradeManager::getCondition() const
{
    return condition;
}

void TradeManager::setCondition(const TradeCondition &val)
{
    condition = val;
}

double TradeManager::getTestProfit() const
{
    return testProfit;
}

std::string TradeManager::getCurDate() const
{
    return curTestDataTime;
}
