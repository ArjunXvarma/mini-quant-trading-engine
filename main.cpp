//
// Created by Arjun Varma on 14/07/25.
//

#include <iostream>
#include <thread>
#include <atomic>

#include "app/include/MarketDataHandler/MarketTick.hpp"
#include "app/include/OrderBook/OrderBook.hpp"
#include "app/include/Trader/Trader.hpp"
#include "app/include/OrderMatchingStrategy/PriceTimeOrderMatchingStrategy.hpp"
#include "app/include/MarketDataHandler/MarketDataReader.hpp"
#include "app/include/Trader/MeanReversionStrategy.hpp"
#include "app/include/Trader/TradingStrategy.hpp"
#include "app/include/MarketDataHandler/ThreadSafeTickQueue.hpp"

void printMarketTickData(const std::vector<MarketTick>& ticks, int n = 5) {
    std::cout << "Printing MarketTick data..." << std::endl;
    std::cout << "Symbol\t" << "Timestamp\t\t\t\t\t" << "Bid Price\t" << "Bid Size\t" << "Sell Price\t" << "Sell Size\n";
    for (int i = 0; i < n; i++) {
        const MarketTick tick = ticks[i];
        std::cout << tick.symbol << "\t";
        std::cout << tick.timestamp << "\t" << tick.bidPrice << "\t\t";
        std::cout << tick.bidSize << "\t\t\t" << tick.askPrice << "\t\t" << tick.askSize << "\n";
    }
}

void traderReplayMarketData(Trader& trader, OrderBook& orderBook, const std::vector<MarketTick>& ticks) {
    std::unique_ptr<TradingStrategy> strategy = std::make_unique<MeanReversionStrategy>(5, 0.03);

    for (const auto& tick : ticks) {
        strategy->onTick(tick, trader, orderBook);

        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void marketDataProducer(MarketDataReader& reader, Stock& stock, ThreadSafeTickQueue& ticks) {
    while (true) {
        auto optTick = reader.readNextTick();
        if (!optTick.has_value()) break; // EOF

        MarketTick tick = optTick.value();
        stock.updateFromTick(tick);
        ticks.enqueue(tick);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void consumeMarketData(ThreadSafeTickQueue& ticks, std::atomic<bool>& done, TradingStrategy& strategy, Trader& trader, OrderBook& orderBook) {
    MarketTick tick;
    while (!done || !ticks.isEmpty()) {
        while (ticks.tryDequeue(tick)) {
            strategy.onTick(tick, trader, orderBook);
        }

        // Avoid busy spinning
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    std::cout << "[Consumer] Finished processing all ticks.\n";
}

int main() {
    std::atomic<bool> done = false;
    ThreadSafeTickQueue tickQueue;

    OrderBook orderBook;
    OrderMatchingStrategy* matchingStrategy = new PriceTimeOrderMatchingStrategy();
    orderBook.setStrategy(matchingStrategy);

    Stock aapl("AAPL", 0.0);

    // std::string filename = "../data/mock_data/MeanReversionStrategyTest.csv";
    std::string filename = "app/data/dxfeed/csv/bid-ask.csv";
    MarketDataReader reader(filename);

    Trader trader("T001");
    std::unique_ptr<TradingStrategy> strategy = std::make_unique<MeanReversionStrategy>(10, 0.03);

    std::thread producer(marketDataProducer, std::ref(reader), std::ref(aapl), std::ref(tickQueue));
    std::thread consumer(consumeMarketData, std::ref(tickQueue), std::ref(done), std::ref(*strategy), std::ref(trader), std::ref(orderBook));

    producer.join();
    done = true;
    consumer.join();

    orderBook.printOrders();

    return 0;
}