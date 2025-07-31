//
// Created by Arjun Varma on 18/07/25.
//

#ifndef MARKETDATAREADER_HPP
#define MARKETDATAREADER_HPP

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>

#include "MarketTick.hpp"
#include "ThreadSafeTickQueue.hpp"

class MarketDataReader {
private:
    std::string filename;
    std::ifstream file;

public:
    explicit MarketDataReader(const std::string& file);
    ~MarketDataReader();

    ThreadSafeTickQueue* readAllTicks();
    std::optional<MarketTick> readNextTick();
};

#endif // MARKETDATAREADER_HPP
