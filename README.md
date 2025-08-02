![C++](https://img.shields.io/badge/language-C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Platform](https://img.shields.io/badge/platform-macOS%20%7C%20Linux-lightgrey?style=for-the-badge&logo=apple&logoColor=black)
![Tests](https://github.com/ArjunXvarma/mini-quant-trading-engine/actions/workflows/ci.yml/badge.svg)

# Mini Quant Trading Engine

## Overview
This project is a miniature quantitative trading engine written in modern C++, designed to simulate trading a single stock using real-world 
market data. It replicates core components of a real trading system, such as order creation, matching logic, and strategy execution, while 
focusing on performance, modularity, and code clarity. The project takes inspiration from 
[this](https://medium.com/@eminalbandyan1/building-a-multi-threaded-stock-trading-system-in-c-3aac8e282c3b) article.

The engine features:
- **Market Data Integration**: Parses real market tick data from CSV files and streams it into the system in simulated real time.
- **Order Management System**: Supports both Market Orders and Limit Orders, with factories to instantiate them and a centralized OrderBook to manage trades.
- **Matching Engine**: Uses a pluggable matching strategy system (e.g., price-time priority) to simulate realistic trade matching.
- **Strategy Engine**: Implements a basic mean reversion trading strategy which reacts to price movements within a sliding window.
- **Multithreading**: Decouples data ingestion and strategy execution using a thread-safe tick queue, simulating concurrent real-time systems.
- **Modular Architecture**: The system is highly extensible, allowing easy swapping or enhancement of components such as strategies or order types.
- **Performance Focus**: Designed with performance in mind, including mutex protection, object reuse, and minimal allocations.

## Project architecture
The system is organised into modular components, each responsible for a particular function in the trading pipeline. This approach allows
for a clear separation of functionality, facilitating smooth testing, performance benchmarking and extensibility. The project contains 4 
modules: 
- Market Data
- Order System 
- Orderbook with matching strategy 
- Trader with trading strategy. 

### Market Data Module
```mermaid
classDiagram 
    class MarketTick {
        +string timestamp
        +string symbol
        +double bidPrice
        +int bidSize
        +double askPrice
        +int askSize
    }

    class MarketDataReader {
        -string filename
        -ifstream file
        +MarketDataReader(string)
        +~MarketDataReader()
        +readAllTicks() ThreadSafeTickQueue*
        +readNextTick() optional<MarketTick>
    }

    class Stock {
        -mutex mutex
        -string symbol
        -string timestamp
        -double price
        -double bid
        -double ask
        +Stock()
        +Stock(string, double)
        +Stock(const Stock&)
        +getMidPrice() double
        +getSymbol() string
        +updateFromTick(tick : MarketTick)
        +getPrice() double
    }

    class ThreadSafeTickQueue {
        -ConcurrentQueue~MarketTick~ queue
        -atomic~bool~ doneProducing
        +enqueue(tick : MarketTick)
        +tryDequeue(outTick : MarketTick) bool
        +tryDequeueMany(outTicks : MarketTick[], maxBatchSize : size_t) bool
        +markDone()
        +isDone() bool
        +isEmpty() bool
    }
    
    Stock --> MarketTick
```

### Order System Module
```mermaid
classDiagram
    class Order {
        <<abstract>>
        -int traderId
        -double price
        -int quantity
        -time_t timestamp
        -OrderType type
        -int orderId
        -static int orderCounter
        +Order(traderId, price, quantity)
        +~Order()
        +getOrderClassType() string
        +getTraderId() int
        +getPrice() double
        +getQuantity() int
        +getTimestamp() time_t
        +getOrderType() OrderType
        +setOrderType(type : OrderType)
        +reduceQuantity(amount : int)
        +getOrderId() int
    }

    class LimitOrder {
        -double limit
        +LimitOrder(traderId, price, quantity, limit)
        +getOrderClassType() string
        +getLimit() double
        +setLimit(limit : double)
    }

    class MarketOrder {
        +MarketOrder(traderId, price, quantity)
        +getOrderClassType() string
    }

    class OrderFactory {
        <<abstract>>
        +createOrder(traderId, price, quantity) Order*
        +~OrderFactory()
    }

    class LimitOrderFactory {
        +createOrder(traderId, price, quantity) Order*
    }

    class MarketOrderFactory {
        +createOrder(traderId, price, quantity) Order*
    }

    Order <|-- LimitOrder
    Order <|-- MarketOrder
    OrderFactory <|-- LimitOrderFactory
    OrderFactory <|-- MarketOrderFactory
```

### Order book and matching strategy
```mermaid
classDiagram
    class OrderMatchingStrategy {
        <<interface>>
        +matchOrders(restingBuyOrders, restingSellOrders, incomingOrder)
        +~OrderMatchingStrategy()
    }

    class PriceTimeOrderMatchingStrategy {
        +matchOrders(restingBuyOrders, restingSellOrders, incomingOrder)
    }

    class OrderBook {
        -vector~shared_ptr<Order>~ buyOrders
        -vector~shared_ptr<Order>~ sellOrders
        -OrderMatchingStrategy* strategy
        -mutex mutex
        -sortOrders()
        +OrderBook()
        +setStrategy(strat : OrderMatchingStrategy*)
        +addOrder(order : shared_ptr<Order>)
        +matchOrder(order : shared_ptr<Order>)
        +printOrders()
        +getAllOrders() vector~shared_ptr<Order>~
        +isEmpty() bool
    }

    PriceTimeOrderMatchingStrategy --|> OrderMatchingStrategy
    OrderBook --> OrderMatchingStrategy : uses *
    OrderBook --> Order : holds *
```

### Trader and trading strategy
```mermaid
classDiagram
    class TradingStrategy {
        <<interface>>
        +onTick(tick : MarketTick, trader : Trader, book : OrderBook)
        +getName() string
        +~TradingStrategy()
    }

    class MeanReversionStrategy {
        -deque~double~ priceWindow
        -size_t windowSize
        -double threshold
        -mutex windowMutex
        +MeanReversionStrategy(windowSize, threshold)
        +onTick(tick, trader, book)
        +getName() string
    }

    class TraderException {
        -string message
        +TraderException(message : string)
        +what() const char*
    }

    class Trader {
        -static int id_counter
        -int id
        -string name
        -Stock stock
        -OrderFactory* orderFactory
        -OrderBook* orderBook
        -vector~shared_ptr<Order>~ orders
        +Trader()
        +Trader(name : string)
        +Trader(name : string, stock : Stock, factory : OrderFactory*, book : OrderBook*)
        +buy(stock : Stock, quantity : int)
        +sell(stock : Stock, quantity : int)
        +trade(...)
        +getID() int
        +getOrders() vector~shared_ptr<Order>~
        +getStock() Stock
        +~Trader()
    }
```

## Modules in action!

### 🧩 System Overview
```mermaid
flowchart TD
    subgraph DataFeed
        MDR[MarketDataReader] --> TSTQ[ThreadSafeTickQueue]
    end

    subgraph Market
        TSTQ --> STK[Stock]
    end

    subgraph TraderLogic
        STK --> TRD[Trader]
        TRD --> TS[TradingStrategy]
        TS --> OB[OrderBook]
        TRD --> OF[OrderFactory]
    end

    subgraph MatchingEngine
        OB --> OMS[OrderMatchingStrategy]
    end
```
