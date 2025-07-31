//
// Created by Arjun Varma on 14/07/25.
//

#ifndef STOCK_HPP
#define STOCK_HPP

#include <string>
#include <mutex>

#include "MarketTick.hpp"

class Stock {
private:
    mutable std::mutex mutex;
    std::string symbol;
    std::string timestamp;
    double price = 0.0;
    double bid = 0.0;
    double ask = 0.0;

public:
    Stock();
    Stock(const std::string& symbol, double price);
    Stock(const Stock& other);

    double getMidPrice();
    std::string getSymbol() const;
    void updateFromTick(const MarketTick& tick);
    double getPrice() const;
};


#endif // STOCK_HPP
