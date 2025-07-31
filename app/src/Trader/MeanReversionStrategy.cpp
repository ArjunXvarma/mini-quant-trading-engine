//
// Created by Arjun Varma on 18/07/25.
//

#include <iostream>
#include <sstream>  // for std::ostringstream

#include "../../include/Trader/MeanReversionStrategy.hpp"
#include "../../include/Utils/Logger.hpp"

MeanReversionStrategy::MeanReversionStrategy(size_t windowSize, double threshold)
    : windowSize(windowSize), threshold(threshold) {}

void MeanReversionStrategy::onTick(const MarketTick& tick, Trader& trader, OrderBook& book) {
    double midPrice = (tick.bidPrice + tick.askPrice) / 2.0;

    std::ostringstream oss;
    oss << "[Tick] Symbol: " << tick.symbol
        << ", Bid: " << tick.bidPrice
        << ", Ask: " << tick.askPrice
        << ", Mid: " << midPrice;
    safeLog(oss.str());

    {
        std::lock_guard<std::mutex> lock(windowMutex);
        priceWindow.push_back(midPrice);
        if (priceWindow.size() > windowSize)
            priceWindow.pop_front();
    }

    // Copy of priceWindow for safe average computation
    std::deque<double> safeCopy;
    {
        std::lock_guard<std::mutex> lock(windowMutex);
        safeCopy = priceWindow;
    }

    if (safeCopy.size() < windowSize) {
        safeLog("[Strategy] Not enough data...");
        return;
    }

    double avg = 0;
    for (double price : priceWindow) avg += price;
    avg /= windowSize;

    safeLog("[Strategy] Moving Avg: " + std::to_string(avg) +
            ", Threshold: " + std::to_string(threshold));

    if (midPrice < avg - threshold) {
        safeLog("[Decision] Mid price (" + std::to_string(midPrice) + ") < avg - threshold (" +
                std::to_string(avg - threshold) + ") → BUY");
        auto buyOrder = std::make_shared<MarketOrder>(trader.getID(), tick.askPrice, 10);
        buyOrder->setOrderType(OrderType::BUY);
        book.addOrder(buyOrder);
    }
    else if (midPrice > avg + threshold) {
        safeLog("[Decision] Mid price (" + std::to_string(midPrice) + ") > avg + threshold (" +
                std::to_string(avg + threshold) + ") → SELL");
        auto sellOrder = std::make_shared<MarketOrder>(trader.getID(), tick.bidPrice, 10);
        sellOrder->setOrderType(OrderType::SELL);
        book.addOrder(sellOrder);
    } else {
        safeLog("[Decision] Mid price within threshold — no trade.");
    }
}
