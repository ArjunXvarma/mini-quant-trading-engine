//
// Created by Arjun Varma on 02/08/25.
//

#include "Benchmark/Benchmark.hpp"

std::vector<MarketTick> generateMarketTicks(size_t count, const std::string& symbol) {
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