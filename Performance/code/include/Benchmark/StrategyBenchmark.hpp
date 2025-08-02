//
// Created by Arjun Varma on 23/07/25.
//

#ifndef STRATEGYBENCHMARK_HPP
#define STRATEGYBENCHMARK_HPP


#include <numeric>

#include "Benchmark.hpp"
#include "Trader/TradingStrategy.hpp"
#include "MetricsExporter.hpp"

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

void runStrategyBenchmark(bool logConsole=true, bool exportCSV=false, bool exportJSON=false, bool exportLatencies=false, const std::string& outputPrefix="Tests/results/performance/strategy");



#endif //STRATEGYBENCHMARK_HPP
