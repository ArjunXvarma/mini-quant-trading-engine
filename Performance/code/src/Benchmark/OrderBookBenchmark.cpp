//
// Created by Arjun Varma on 04/08/25.
//

#include "../../include/Benchmark/OrderBookBenchmark.hpp"

#include <chrono>
#include <thread>
#include <numeric>
#include <algorithm>
#include <random>
#include <iostream>

OrderBookBenchmark::OrderBookBenchmark(size_t orders, int threads)
    : numOrders(orders), numThreads(threads) {}

std::string OrderBookBenchmark::getName() const {
    return "OrderBookBenchmark";
}

std::shared_ptr<Order> OrderBookBenchmark::generateRandomOrder(int traderId) {
    static thread_local std::default_random_engine engine(std::random_device{}());
    std::uniform_real_distribution<double> priceDist(100.0, 200.0);
    std::uniform_int_distribution<int> quantityDist(1, 100);
    double price = priceDist(engine);
    int quantity = quantityDist(engine);
    return std::make_shared<MarketOrder>(traderId, price, quantity);
}

BenchmarkResult OrderBookBenchmark::runBenchmark() {
    OrderBook book;
    PriceTimeOrderMatchingStrategy strategy;
    book.setStrategy(&strategy);

    latencies.clear();
    std::mutex latencyMutex;

    auto startTime = std::chrono::high_resolution_clock::now();

    auto worker = [&](int threadId, int ordersPerThread) {
        for (int i = 0; i < ordersPerThread; ++i) {
            auto order = generateRandomOrder(threadId * ordersPerThread + i);
            auto start = std::chrono::high_resolution_clock::now();
            book.addOrder(order);
            auto end = std::chrono::high_resolution_clock::now();
            long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            std::lock_guard<std::mutex> lock(latencyMutex);
            latencies.push_back(duration);
        }
    };

    std::vector<std::thread> threads;
    int ordersPerThread = static_cast<int>(numOrders / numThreads);
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(worker, i, ordersPerThread);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    double totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    double throughput = (double)numOrders / (totalDuration / 1e6);

    std::sort(latencies.begin(), latencies.end());

    auto percentile = [&](double p) {
        return latencies[(size_t)(p * latencies.size())];
    };

    return BenchmarkResult{
        getName(),
        (double)std::accumulate(latencies.begin(), latencies.end(), 0LL) / latencies.size() / 1000.0,
        percentile(0.95) / 1000.0,
        percentile(0.99) / 1000.0,
        latencies.back() / 1000.0,
        throughput
    };
}

void runOrderBookBenchmark(bool logConsole, bool exportCSV, bool exportJSON, bool exportLatencies, const std::string& outputPrefix) {
    Benchmark* benchmark = new OrderBookBenchmark(1000000, std::thread::hardware_concurrency());
    BenchmarkResult result = benchmark->runBenchmark();

    if (logConsole) MetricsExporter::logToConsole({result});
    if (exportCSV) MetricsExporter::exportToCSV(outputPrefix + ".csv", {result});
    if (exportJSON) MetricsExporter::exportToJSON(outputPrefix + ".json", {result});

    if (exportLatencies) {
        std::vector<long long> latencies = benchmark->getLatencies();
        MetricsExporter::exportLatenciesToCSV(latencies, outputPrefix + "Latencies.csv");
    }

    delete benchmark;
}

