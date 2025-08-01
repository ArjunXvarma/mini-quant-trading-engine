//
// Created by Arjun Varma on 23/07/25.
//

#include "../../include/Benchmark/StrategyBenchmark.hpp"

#include "Trader/MeanReversionStrategy.hpp"

StrategyBenchmark::StrategyBenchmark(TradingStrategy& s, Trader& t, OrderBook& b, std::vector<MarketTick> ticks)
    : strategy(s), trader(t), orderBook(b), marketTicks(std::move(ticks)) {}

std::string StrategyBenchmark::getName() const {
    return "StrategyBenchmark_" + strategy.getName();
}

BenchmarkResult StrategyBenchmark::runBenchmark() {
    latencies.reserve(marketTicks.size());

    auto startThroughput = std::chrono::high_resolution_clock::now();

    for (const auto& tick : marketTicks) {
        auto start = std::chrono::high_resolution_clock::now();
        strategy.onTick(tick, trader, orderBook);
        auto end = std::chrono::high_resolution_clock::now();

        latencies.push_back(std::chrono::duration<double, std::micro>(end - start).count());
    }

    auto endThroughput = std::chrono::high_resolution_clock::now();
    double totalDuration = std::chrono::duration<double>(endThroughput - startThroughput).count();

    std::sort(latencies.begin(), latencies.end());

    BenchmarkResult result;
    result.name = getName();
    result.avgLatency = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();
    result.p95Latency = latencies[static_cast<size_t>(0.95 * latencies.size())];
    result.p99Latency = latencies[static_cast<size_t>(0.99 * latencies.size())];
    result.maxLatency = latencies.back();
    result.throughput = latencies.size() / totalDuration;

    return result;
}

void runStrategyBenchmark(bool logConsole, bool exportCSV, bool exportJSON, bool exportLatencies, const std::string& outputPrefix) {
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