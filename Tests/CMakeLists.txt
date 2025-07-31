cmake_minimum_required(VERSION 3.14)

project(Engine_tests)

# -----------------------------
# Fetch GoogleTest via FetchContent
# -----------------------------
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# This makes gtest and gtest_main targets available
FetchContent_MakeAvailable(googletest)

# -----------------------------
# Include Project Headers
# -----------------------------
include_directories(
        ../app/include
)

# -----------------------------
# Source Files from Application
# -----------------------------
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

# -----------------------------
# Test Source Files
# -----------------------------
set(TEST_FILES
        orderMatchingStrategy.cpp
        tradingStrategy.cpp
        tickQueue.cpp
)

# -----------------------------
# Build Test Executable
# -----------------------------
add_executable(Engine_tests ${TEST_FILES} ${SOURCE_FILES})

# Link with GoogleTest
target_link_libraries(Engine_tests gtest gtest_main)
