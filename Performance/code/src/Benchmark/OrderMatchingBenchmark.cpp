//
// Created by Arjun Varma on 23/07/25.
//

#include "../../include/Benchmark/OrderMatchingBenchmark.hpp"

#include <numeric>

#include "OrderBook/OrderBook.hpp"
#include "Orders/MarketOrder.hpp"

#define ORDER_COUNT 1000000

OrderMatchingBenchmark::OrderMatchingBenchmark(OrderMatchingStrategy &strategy)
    : strategy(strategy) {
    book.setStrategy(&strategy);
}


void OrderMatchingBenchmark::generateSyntheticOrders(int count) {
    for (int i = 0; i < count; ++i) {
        double price = 100.0 + (i % 10);
        int quantity = 10 + (i % 5);
        OrderType type = (i % 2 == 0) ? OrderType::BUY : OrderType::SELL;
        auto order = orderFactory->createOrder(1, price, quantity);
        order->setOrderType(type);

        auto t1 = std::chrono::high_resolution_clock::now();
        book.matchOrder(order);
        auto t2 = std::chrono::high_resolution_clock::now();
        latencies.push_back(std::chrono::duration<double, std::micro>(t2 - t1).count());
    }
}

BenchmarkResult OrderMatchingBenchmark::runBenchmark() {
    int orderCount = ORDER_COUNT;
    auto start = std::chrono::high_resolution_clock::now();

    generateSyntheticOrders(orderCount);

    auto end = std::chrono::high_resolution_clock::now();
    double totalTime = std::chrono::duration<double>(end - start).count();

    // Sort and calculate percentiles
    std::sort(latencies.begin(), latencies.end());
    BenchmarkResult result;
    result.name = getName();
    result.avgLatency = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();
    result.p95Latency = latencies[int(latencies.size() * 0.95)];
    result.p99Latency = latencies[int(latencies.size() * 0.99)];
    result.maxLatency = latencies.back();
    result.throughput = orderCount / totalTime;

    return result;
}

