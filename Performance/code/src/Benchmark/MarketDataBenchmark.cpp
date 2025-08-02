//
// Created by Arjun Varma on 23/07/25.
//

#include "../../include/Benchmark/MarketDataBenchmark.hpp"

#include <numeric>

#include "MarketDataHandler/ThreadSafeTickQueue.hpp"

std::string MarketDataBenchmark::getName() const {
    return "MarketDataBenchmark_ThreadSafeTickQueue";
}

BenchmarkResult MarketDataBenchmark::runBenchmark() {
    constexpr int producerCount = 4;
    constexpr int consumerCount = 4;
    constexpr int ticksPerProducer = 250'000;

    std::atomic<int> totalOperations = 0;
    ThreadSafeTickQueue queue;

    auto generateTick = [](int id) {
        MarketTick tick;
        tick.symbol = "AAPL";
        tick.timestamp = std::to_string(id);
        tick.bidPrice = 100.0 + (id % 10);
        tick.askPrice = 100.5 + (id % 10);
        tick.bidSize = 100 + (id % 20);
        tick.askSize = 100 + (id % 20);
        return tick;
    };

    auto benchmarkStart = std::chrono::high_resolution_clock::now();

    // --- Producers ---
    std::vector<std::thread> producers;
    for (int i = 0; i < producerCount; ++i) {
        producers.emplace_back([&, i]() {
            for (int j = 0; j < ticksPerProducer; ++j) {
                MarketTick tick = generateTick(i * ticksPerProducer + j);
                auto start = std::chrono::high_resolution_clock::now();
                queue.enqueue(tick);
                auto end = std::chrono::high_resolution_clock::now();

                {
                    std::lock_guard<std::mutex> lock(LatencyMutex);
                    latencies.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
                }

                totalOperations.fetch_add(1);
            }
        });
    }

    // --- Consumers ---
    std::vector<std::thread> consumers;
    for (int i = 0; i < consumerCount; ++i) {
        consumers.emplace_back([&]() {
            MarketTick tick;
            while (totalOperations.load() < producerCount * ticksPerProducer || !queue.isEmpty()) {
                auto start = std::chrono::high_resolution_clock::now();
                if (queue.tryDequeue(tick)) {
                    auto end = std::chrono::high_resolution_clock::now();

                    {
                        std::lock_guard<std::mutex> lock(LatencyMutex);
                        latencies.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
                    }

                    totalOperations.fetch_add(1);
                }
            }
        });
    }

    // Join all threads
    for (auto& p : producers) p.join();
    queue.markDone();
    for (auto& c : consumers) c.join();

    // --- Metrics ---
    auto benchmarkEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> wallClockDuration = benchmarkEnd - benchmarkStart;
    double totalTimeSec = wallClockDuration.count();

    size_t count = latencies.size();
    if (count == 0) count = 1;

    std::sort(latencies.begin(), latencies.end());
    double avg = std::accumulate(latencies.begin(), latencies.end(), 0.0) / count;
    double p95 = latencies[size_t(0.95 * count)];
    double p99 = latencies[size_t(0.99 * count)];
    double max = *std::max_element(latencies.begin(), latencies.end());

    double throughput = (producerCount * ticksPerProducer) / totalTimeSec;

    return {
        getName(),
        avg / 1000.0,   // μs
        p95 / 1000.0,   // μs
        p99 / 1000.0,   // μs
        max / 1000.0,   // μs
        throughput
    };
}

void runMarketDataBenchmark(bool logConsole, bool exportCSV, bool exportJSON, bool exportLatencies, const std::string& outputPrefix) {
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
