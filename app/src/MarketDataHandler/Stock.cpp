//
// Created by Arjun Varma on 14/07/25.
//

#include "../../include/MarketDataHandler/Stock.hpp"

Stock::Stock() {

}

Stock::Stock(const std::string& symbol, double price)
    : symbol(symbol), price(price) {

}

Stock::Stock(const Stock& other)
    : symbol(other.symbol), timestamp(other.timestamp), price(other.price), bid(other.bid), ask(other.ask) {

}

double Stock::getMidPrice() {
    std::lock_guard<std::mutex> lock(mutex);
    return (bid + ask) / 2.0;
}

std::string Stock::getSymbol() const {
    return symbol;
}

void Stock::updateFromTick(const MarketTick& tick) {
    std::lock_guard<std::mutex> lock(mutex);
    this->price = (tick.bidPrice + tick.askPrice) / 2.0;
    this->bid = tick.bidPrice;
    this->ask = tick.askPrice;
    this->timestamp = tick.timestamp;
}

double Stock::getPrice() const {
    return price;
}
