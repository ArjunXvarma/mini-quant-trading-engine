//
// Created by Arjun Varma on 23/07/25.
//

#ifndef MARKETDATABENCHMARK_HPP
#define MARKETDATABENCHMARK_HPP

#include <thread>
#include <chrono>
#include <mutex>

#include "MetricsExporter.hpp"
#include "../Benchmark/Benchmark.hpp"

class MarketDataBenchmark : public Benchmark {
private:
    std::mutex LatencyMutex;
public:
    std::string getName() const override;
    BenchmarkResult runBenchmark() override;
};

void runMarketDataBenchmark(bool logConsole=true, bool exportCSV=false, bool exportJSON=false, bool exportLatencies=false, const std::string& outputPrefix="Tests/results/performance/marketdata");

#endif // MARKETDATABENCHMARK_HPP

