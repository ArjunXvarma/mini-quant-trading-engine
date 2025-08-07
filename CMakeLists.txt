cmake_minimum_required(VERSION 3.31)
project(Order_Book_Engine)

set(CMAKE_CXX_STANDARD 20)

include_directories(app/include)

add_executable(Quant_Engine
        main.cpp
        app/src/Trader/Trader.cpp
        app/include/Trader/Trader.hpp
        app/src/MarketDataHandler/Stock.cpp
        app/include/MarketDataHandler/Stock.hpp
        app/src/Orders/Order.cpp
        app/include/Orders/Order.hpp
        app/src/Orders/MarketOrder.cpp
        app/include/Orders/MarketOrder.hpp
        app/src/Orders/LimitOrder.cpp
        app/include/Orders/LimitOrder.hpp
        app/include/Orders/OrderFactory.hpp
        app/include/OrderMatchingStrategy/OrderMatchingStrategy.hpp
        app/src/OrderMatchingStrategy/PriceTimeOrderMatchingStrategy.cpp
        app/include/OrderMatchingStrategy/PriceTimeOrderMatchingStrategy.hpp
        app/src/OrderBook/OrderBook.cpp
        app/include/OrderBook/OrderBook.hpp
        app/include/Orders/OrderType.hpp
        app/src/Orders/LimitOrderFactory.cpp
        app/include/Orders/LimitOrderFactory.hpp
        app/src/Orders/MarketOrderFactory.cpp
        app/include/Orders/MarketOrderFactory.hpp
        app/src/MarketDataHandler/MarketDataReader.cpp
        app/include/MarketDataHandler/MarketDataReader.hpp
        app/include/MarketDataHandler/MarketTick.hpp
        app/include/Trader/TradingStrategy.hpp
        app/src/Trader/MeanReversionStrategy.cpp
        app/include/Trader/MeanReversionStrategy.hpp
        app/include/Utils/Logger.hpp
        app/src/Utils/Logger.cpp
        app/include/ConcurrentQueue/concurrentqueue.hpp
        app/include/ConcurrentQueue/lightweightsemaphore.hpp
        app/include/MarketDataHandler/ThreadSafeTickQueue.hpp
)
