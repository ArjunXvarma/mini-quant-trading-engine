cmake_minimum_required(VERSION 3.14)

project(Engine_tests)

add_subdirectory(lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(
    ../app/include
)

set(SOURCE_FILES
    ../app/src/MarketDataHandler/Stock.cpp
    ../app/src/OrderBook/OrderBook.cpp
    ../app/src/Orders/Order.cpp
    ../app/src/Orders/LimitOrder.cpp
    ../app/src/Orders/MarketOrder.cpp
    ../app/src/OrderMatchingStrategy/OrderMatchingStrategy.cpp
    ../app/src/OrderMatchingStrategy/PriceTimeOrderMatchingStrategy.cpp
    ../app/src/Trader/Trader.cpp
    ../app/src/MarketDataHandler/MarketDataReader.cpp
    ../app/src/MarketDataHandler/MarketTick.cpp
    ../app/src/Trader/MeanReversionStrategy.cpp
)

set(TEST_FILES
    orderMatchingStrategy.cpp
    tradingStrategy.cpp
    tickQueue.cpp
)

add_executable(Engine_tests ${TEST_FILES} ${SOURCE_FILES})

target_link_libraries(Engine_tests gtest gtest_main)