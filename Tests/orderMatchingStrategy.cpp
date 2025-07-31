//
// Created by Arjun Varma on 30/07/25.
//

#include <gtest/gtest.h>
#include "OrderBook/OrderBook.hpp"
#include "Orders/LimitOrder.hpp"
#include "OrderMatchingStrategy/PriceTimeOrderMatchingStrategy.hpp"

TEST(OrderMatchingTest, ExactPriceMatch) {
    auto strategy = new PriceTimeOrderMatchingStrategy();
    OrderBook book;
    book.setStrategy(strategy);

    auto buyOrder = std::make_shared<LimitOrder>(1, 100.0, 100, 10);
    buyOrder->setOrderType(OrderType::BUY);
    auto sellOrder = std::make_shared<LimitOrder>(2, 100.0, 100, 10);
    sellOrder->setOrderType(OrderType::SELL);

    // Use addOrder for both to trigger matching logic correctly
    book.addOrder(buyOrder);
    book.addOrder(sellOrder);

    // After perfect match, order book should be empty
    EXPECT_TRUE(book.isEmpty());

    delete strategy;
}

TEST(OrderMatchingTest, NoMatchDueToPrice) {
    auto strategy = new PriceTimeOrderMatchingStrategy();
    OrderBook book;
    book.setStrategy(strategy);

    auto buyOrder = std::make_shared<LimitOrder>(1, 100.0, 100.0, 10);
    auto sellOrder = std::make_shared<LimitOrder>(2, 101.0, 101.0, 10);
    buyOrder->setOrderType(OrderType::BUY);
    sellOrder->setOrderType(OrderType::SELL);

    book.addOrder(buyOrder);
    book.matchOrder(sellOrder);

    // No match due to price; both orders stay
    EXPECT_FALSE(book.isEmpty());
    delete strategy;
}

TEST(OrderMatchingTest, PartialMatch) {
    auto strategy = new PriceTimeOrderMatchingStrategy();
    OrderBook book;
    book.setStrategy(strategy);

    auto buyOrder = std::make_shared<LimitOrder>(1, 100.0, 10, 100.0);
    auto sellOrder = std::make_shared<LimitOrder>(2, 100.0, 5, 100.0);

    buyOrder->setOrderType(OrderType::BUY);
    sellOrder->setOrderType(OrderType::SELL);

    book.addOrder(buyOrder);
    book.matchOrder(sellOrder);

    // Buy order still has quantity left
    EXPECT_FALSE(book.isEmpty());
    delete strategy;
}
