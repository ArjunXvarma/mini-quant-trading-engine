//
// Created by Arjun Varma on 04/08/25.
//

#ifndef ORDERBOOKBENCHMARK_HPP
#define ORDERBOOKBENCHMARK_HPP



#include <mutex>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include <random>

#include "Benchmark.hpp"
#include "OrderBook/OrderBook.hpp"
#include "OrderMatchingStrategy/PriceTimeOrderMatchingStrategy.hpp"
#include "Orders/MarketOrder.hpp"
#include "Orders/Order.hpp"
#include "MetricsExporter.hpp"

class OrderBookBenchmark : public Benchmark {
private:
    size_t numOrders;
    int numThreads;

    std::shared_ptr<Order> generateRandomOrder(int traderId);

public:
    OrderBookBenchmark(size_t orders = 100000, int threads = 4);

    std::string getName() const override;
    BenchmarkResult runBenchmark() override;
};

// Launcher function
void runOrderBookBenchmark(bool logConsole = true,
                           bool exportCSV = false,
                           bool exportJSON = false,
                           bool exportLatencies = false,
                           const std::string& outputPrefix = "Performance/results/performance/orderbook");



#endif //ORDERBOOKBENCHMARK_HPP