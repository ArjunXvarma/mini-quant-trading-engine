//
// Created by Arjun Varma on 18/07/25.
//

#ifndef MEANREVERSIONSTRATEGY_HPP
#define MEANREVERSIONSTRATEGY_HPP

#include <deque>
#include <mutex>

#include "TradingStrategy.hpp"
#include "../Orders/MarketOrder.hpp"

class MeanReversionStrategy : public TradingStrategy {
private:
    std::deque<double> priceWindow;
    size_t windowSize;
    double threshold;
    std::mutex windowMutex;

public:
    MeanReversionStrategy(size_t windowSize, double threshold);

    void onTick(const MarketTick &tick, Trader &trader, OrderBook &book) override;

    std::string getName() const override {
        return "MeanReversionStrategy";
    }
};

#endif // MEANREVERSIONSTRATEGY_HPP

