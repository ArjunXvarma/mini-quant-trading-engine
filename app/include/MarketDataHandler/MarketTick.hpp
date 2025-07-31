//
// Created by Arjun Varma on 18/07/25.
//

#ifndef MARKETTICK_HPP
#define MARKETTICK_HPP

#include <string>
#include <ctime>

struct MarketTick {
    std::string timestamp;
    std::string symbol;
    double bidPrice;
    int bidSize;
    double askPrice;
    int askSize;
};


#endif //MARKETTICK_HPP
