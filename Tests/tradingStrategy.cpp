//
// Created by Arjun Varma on 30/07/25.
//

#include <gtest/gtest.h>
#include "OrderBook/OrderBook.hpp"
#include "Trader/Trader.hpp"
#include "MarketDataHandler/MarketTick.hpp"
#include "Trader/MeanReversionStrategy.hpp"

#include "OrderMatchingStrategy/PriceTimeOrderMatchingStrategy.hpp"

class MeanReversionStrategyTest : public ::testing::Test {
protected:
    Trader trader;
    OrderBook book;
    MeanReversionStrategy strategy{3, 5.0};  // windowSize = 3, threshold = 5.0
    std::unique_ptr<PriceTimeOrderMatchingStrategy> matchingStrategy;

    void SetUp() override {
        matchingStrategy = std::make_unique<PriceTimeOrderMatchingStrategy>();
        book.setStrategy(matchingStrategy.get());
    }

    MarketTick makeTick(double bid, double ask) {
        MarketTick tick;
        tick.symbol = "AAPL";
        tick.bidPrice = bid;
        tick.askPrice = ask;
        return tick;
    }

    const int getOrdersSize() {
        return book.getAllOrders().size();
    }

    const std::shared_ptr<Order> getOrder(int index) {
        return book.getAllOrders()[index];
    }
};

TEST_F(MeanReversionStrategyTest, NoTradeWithInsufficientData) {
    MarketTick tick1 = makeTick(99.0, 101.0);  // Mid = 100
    strategy.onTick(tick1, trader, book);

    ASSERT_EQ(getOrdersSize(), 0);
}

TEST_F(MeanReversionStrategyTest, BuysBelowThreshold) {
    // Assuming windowSize = 3, threshold = 5.0
    strategy.onTick(makeTick(98.0, 102.0), trader, book); // Mid = 100
    strategy.onTick(makeTick(97.0, 101.0), trader, book); // Mid = 99
    strategy.onTick(makeTick(99.0, 101.0), trader, book); // Mid = 100, Avg = 99.67

    // Mid = 91, Avg = 99.67 → 91 < 94.67 → triggers BUY
    strategy.onTick(makeTick(90.0, 92.0), trader, book);  // Mid = 91

    ASSERT_EQ(getOrdersSize(), 1);
    EXPECT_EQ(getOrder(0)->getOrderType(), OrderType::BUY);
}

TEST_F(MeanReversionStrategyTest, SellsAboveThreshold) {
    // Assuming windowSize = 3, threshold = 5.0
    strategy.onTick(makeTick(98.0, 102.0), trader, book); // Mid = 100
    strategy.onTick(makeTick(97.0, 101.0), trader, book); // Mid = 99
    strategy.onTick(makeTick(99.0, 101.0), trader, book); // Mid = 100, Avg = 99.67

    // Mid = 111, Avg = 99.67 → 111 > 104.67 → triggers SELL
    strategy.onTick(makeTick(110.0, 112.0), trader, book);  // Mid = 111

    ASSERT_EQ(getOrdersSize(), 1);
    EXPECT_EQ(getOrder(0)->getOrderType(), OrderType::SELL);
}

TEST_F(MeanReversionStrategyTest, NoActionWithinThreshold) {
    strategy.onTick(makeTick(98.0, 102.0), trader, book); // Mid = 100
    strategy.onTick(makeTick(97.0, 101.0), trader, book); // Mid = 99
    strategy.onTick(makeTick(99.0, 101.0), trader, book); // Mid = 100, Avg = 99.67

    // Final tick within ±5 of avg → NO ACTION
    strategy.onTick(makeTick(97.0, 103.0), trader, book);  // Mid = 100

    ASSERT_EQ(getOrdersSize(), 0);
}
