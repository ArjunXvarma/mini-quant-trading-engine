//
// Created by Arjun Varma on 18/07/25.
//

#ifndef TRADINGSTRATEGY_HPP
#define TRADINGSTRATEGY_HPP



#include "../MarketDataHandler/MarketTick.hpp"
#include "Trader.hpp"

class TradingStrategy {
public:
    virtual void onTick(const MarketTick& tick, Trader& trader, OrderBook& book) = 0;
    virtual std::string getName() const = 0;
    virtual ~TradingStrategy() = default;
};



#endif //TRADINGSTRATEGY_HPP
