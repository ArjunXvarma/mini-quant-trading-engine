//
// Created by Arjun Varma on 20/07/25.
//

#include <random>
#include <string>
#include <chrono>
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

#include "include/OrderBook/OrderBook.hpp"
#include "../../include/OrderMatchingStrategy/PriceTimeOrderMatchingStrategy.hpp"
#include "include/Benchmark/OrderMatchingBenchmark.hpp"
#include "include/Benchmark/Benchmark.hpp"
#include "include/Benchmark/MetricsExporter.hpp"
#include "include/Benchmark/StrategyBenchmark.hpp"
#include "Trader/MeanReversionStrategy.hpp"
#include "../include/MarketDataHandler/MarketTick.hpp"
#include "include/Benchmark/MarketDataBenchmark.hpp"

std::vector<MarketTick> generateMarketTicks(size_t count, const std::string& symbol = "AAPL") {
    std::vector<MarketTick> ticks;
    ticks.reserve(count);

    std::default_random_engine rng(std::random_device{}());
    std::uniform_real_distribution<double> bidPriceDist(90.0, 100.0);
    std::uniform_real_distribution<double> spreadDist(0.01, 0.5);  // ask = bid + spread
    std::uniform_int_distribution<int> sizeDist(1, 100);

    for (size_t i = 0; i < count; ++i) {
        double bidPrice = bidPriceDist(rng);
        double askPrice = bidPrice + spreadDist(rng);
        int bidSize = sizeDist(rng);
        int askSize = sizeDist(rng);

        // Generate timestamp in ISO 8601 format
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
        std::tm* gmt = std::gmtime(&nowTime);
        std::stringstream timestampStream;
        timestampStream << std::put_time(gmt, "%FT%TZ");

        ticks.push_back({
            .timestamp = timestampStream.str(),
            .symbol = symbol,
            .bidPrice = bidPrice,
            .bidSize = bidSize,
            .askPrice = askPrice,
            .askSize = askSize
        });
    }

    return ticks;
}

void runOrderMatchingBenchmark(bool logConsole = true, bool exportCSV = false, bool exportJSON = false, bool exportLatencies = false, const std::string& outputPrefix = "Tests/results/performance/order_matching") {
    auto* strategy = new PriceTimeOrderMatchingStrategy();
    Benchmark* benchmark = new OrderMatchingBenchmark(*strategy);
    BenchmarkResult result = benchmark->runBenchmark();

    if (logConsole) MetricsExporter::logToConsole({result});
    if (exportCSV) MetricsExporter::exportToCSV(outputPrefix + ".csv", {result});
    if (exportJSON) MetricsExporter::exportToJSON(outputPrefix + ".json", {result});

    if (exportLatencies) {
        std::vector<long long> latencies = benchmark->getLatencies();
        MetricsExporter::exportLatenciesToCSV(latencies, "Performance/results/performance/OrderMatchingLatencies.csv");
    }

    delete strategy;
    delete benchmark;
}

void runStrategyBenchmark(bool logConsole = true, bool exportCSV = false, bool exportJSON = false, bool exportLatencies = false, const std::string& outputPrefix = "Tests/results/performance/strategy") {
    // Prepare dependencies
    Trader trader;
    OrderBook orderBook;
    std::vector<MarketTick> ticks = generateMarketTicks(1000);
    size_t windowSize = 5;
    double threshold = 0.5;

    MeanReversionStrategy strategy(windowSize, threshold);

    Benchmark* benchmark = new StrategyBenchmark(strategy, trader, orderBook, ticks);
    BenchmarkResult result = benchmark->runBenchmark();

    if (logConsole) MetricsExporter::logToConsole({result});
    if (exportCSV) MetricsExporter::exportToCSV(outputPrefix + ".csv", {result});
    if (exportJSON) MetricsExporter::exportToJSON(outputPrefix + ".json", {result});

    if (exportLatencies) {
        std::vector<long long> latencies = benchmark->getLatencies();
        MetricsExporter::exportLatenciesToCSV(latencies, "Performance/results/performance/StrategyLatencies.csv");
    }

    delete benchmark;
}

void runMarketDataBenchmark(bool logConsole = true, bool exportCSV = false, bool exportJSON = false, bool exportLatencies = false, const std::string& outputPrefix = "Tests/results/performance/marketdata") {
    Benchmark* benchmark = new MarketDataBenchmark();
    BenchmarkResult result = benchmark->runBenchmark();

    if (logConsole) MetricsExporter::logToConsole({result});
    if (exportCSV) MetricsExporter::exportToCSV(outputPrefix + ".csv", {result});
    if (exportJSON) MetricsExporter::exportToJSON(outputPrefix + ".json", {result});

    if (exportLatencies) {
        std::vector<long long> latencies = benchmark->getLatencies();
        MetricsExporter::exportLatenciesToCSV(latencies, "Performance/results/performance/MarketDataLatencies.csv");
    }

    delete benchmark;
}

int main() {
    // runner.reportResultsToCSV("Tests/results/performance/benchmark_results.csv");

    runOrderMatchingBenchmark(
        /*logConsole=*/true,
        /*exportCSV=*/false,
        /*exportJSON=*/false,
        /*exportLatencies=*/true
    );

    runStrategyBenchmark(
        /*logConsole=*/true,
        /*exportCSV=*/false,
        /*exportJSON=*/false,
        /*exportLatencies=*/true
    );

    runMarketDataBenchmark(
        /*logConsole=*/true,
        /*exportCSV=*/false,
        /*exportJSON=*/false,
        /*exportLatencies=*/true
    );

    return 0;
}
