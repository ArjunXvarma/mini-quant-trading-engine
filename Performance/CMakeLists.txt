cmake_minimum_required(VERSION 3.31)
project(Performance_test)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        ../app/include
        code/include
)

add_executable(Performance_test
        code/main.cpp
        ../app/src/Trader/Trader.cpp
        ../app/src/MarketDataHandler/Stock.cpp
        ../app/src/Orders/Order.cpp
        ../app/src/Orders/MarketOrder.cpp
        ../app/src/Orders/LimitOrder.cpp
        ../app/src/Orders/OrderFactory.cpp
        ../app/src/OrderMatchingStrategy/PriceTimeOrderMatchingStrategy.cpp
        ../app/src/OrderBook/OrderBook.cpp
        ../app/src/Orders/LimitOrderFactory.cpp
        ../app/src/Orders/MarketOrderFactory.cpp
        ../app/src/MarketDataHandler/MarketDataReader.cpp
        ../app/src/Trader/MeanReversionStrategy.cpp
        ../app/src/Utils/Logger.cpp

        code/src/Benchmark/OrderMatchingBenchmark.cpp
        code/src/Benchmark/MetricsExporter.cpp
        code/src/Benchmark/MarketDataBenchmark.cpp
        code/src/Benchmark/StrategyBenchmark.cpp
        code/src/Benchmark/Benchmark.cpp
        code/src/Benchmark/OrderBookBenchmark.cpp
        code/include/Benchmark/OrderBookBenchmark.hpp
)
