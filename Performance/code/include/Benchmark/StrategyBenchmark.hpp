//
// Created by Arjun Varma on 23/07/25.
//

#ifndef STRATEGYBENCHMARK_HPP
#define STRATEGYBENCHMARK_HPP


#include <numeric>

#include "Benchmark.hpp"
#include "Trader/TradingStrategy.hpp"

class StrategyBenchmark : public Benchmark {
private:
    TradingStrategy& strategy;
    Trader& trader;
    OrderBook& orderBook;
    std::vector<MarketTick> marketTicks;

public:
    StrategyBenchmark(TradingStrategy& s, Trader& t, OrderBook& b, std::vector<MarketTick> ticks);

    std::string getName() const override;
    BenchmarkResult runBenchmark() override;
};




#endif //STRATEGYBENCHMARK_HPP
