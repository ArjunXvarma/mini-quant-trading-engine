//
// Created by Arjun Varma on 20/07/25.
//

#include "include/Benchmark/OrderMatchingBenchmark.hpp"
#include "include/Benchmark/StrategyBenchmark.hpp"
#include "../include/MarketDataHandler/MarketTick.hpp"
#include "include/Benchmark/MarketDataBenchmark.hpp"

int main() {
    runOrderMatchingBenchmark(
        /*logConsole=*/true,
        /*exportCSV=*/false,
        /*exportJSON=*/false,
        /*exportLatencies=*/false
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
        /*exportLatencies=*/false
    );

    return 0;
}
