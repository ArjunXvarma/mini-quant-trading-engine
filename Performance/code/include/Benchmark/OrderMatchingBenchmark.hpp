//
// Created by Arjun Varma on 23/07/25.
//

#ifndef ORDERMATCHINGBENCHMARK_HPP
#define ORDERMATCHINGBENCHMARK_HPP



#include "Benchmark.hpp"
#include <vector>
#include <chrono>

#include "OrderBook/OrderBook.hpp"
#include "Orders/MarketOrderFactory.hpp"
#include "Orders/OrderFactory.hpp"
#include "MetricsExporter.hpp"


class OrderMatchingBenchmark : public Benchmark {
private:
    OrderMatchingStrategy &strategy;
    std::unique_ptr<OrderFactory> orderFactory = std::make_unique<MarketOrderFactory>();
    OrderBook book;

public:
    OrderMatchingBenchmark(OrderMatchingStrategy& strategy);
    std::string getName() const override { return "OrderMatchingBenchmark"; }

    BenchmarkResult runBenchmark() override;
    void generateSyntheticOrders(int count);
};

void runOrderMatchingBenchmark(bool logConsole=true, bool exportCSV=false, bool exportJSON=false, bool exportLatencies=false, const std::string& outputPrefix="Tests/results/performance/order_matching");


#endif //ORDERMATCHINGBENCHMARK_HPP
