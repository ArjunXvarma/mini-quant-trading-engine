//
// Created by Arjun Varma on 18/07/25.
//

#include "../../include/MarketDataHandler/MarketDataReader.hpp"

#include "MarketDataHandler/ThreadSafeTickQueue.hpp"

MarketDataReader::MarketDataReader(const std::string& filename) : filename(filename), file(filename) {
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::string line;
    std::getline(file, line);
}

MarketDataReader::~MarketDataReader() {
    if (file.is_open()) file.close();
}

ThreadSafeTickQueue* MarketDataReader::readAllTicks() {
    ThreadSafeTickQueue ticks;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 11) continue;
        MarketTick tick;
        tick.symbol    = tokens[1];
        tick.timestamp = tokens[2];
        tick.bidPrice  = std::stod(tokens[5]);
        tick.bidSize   = std::stoi(tokens[6]);
        tick.askPrice  = std::stod(tokens[9]);
        tick.askSize   = std::stoi(tokens[10]);

        ticks.enqueue(tick);
    }

    return &ticks;
}

std::optional<MarketTick> MarketDataReader::readNextTick() {
    std::string line;
    if (!std::getline(file, line)) {
        return std::nullopt;
    }

    std::istringstream iss(line);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(iss, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 11) return std::nullopt;

    MarketTick tick;
    tick.symbol    = tokens[1];
    tick.timestamp = tokens[2];
    tick.bidPrice  = std::stod(tokens[5]);
    tick.bidSize   = std::stoi(tokens[6]);
    tick.askPrice  = std::stod(tokens[9]);
    tick.askSize   = std::stoi(tokens[10]);

    return tick;
}


