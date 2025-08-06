//
// Created by Arjun Varma on 06/08/25.
//
#include <gtest/gtest.h>
#include "OrderBook/OrderBook.hpp"
#include "Orders/LimitOrder.hpp"
#include "OrderMatchingStrategy/PriceTimeOrderMatchingStrategy.hpp"

TEST(OrderBookTest, AddSingleBuyOrder) {
    auto strategy = new PriceTimeOrderMatchingStrategy();
    OrderBook book;
    book.setStrategy(strategy);

    auto order = std::make_shared<LimitOrder>(1, 100.0, 50, 100.0);
    order->setOrderType(OrderType::BUY);

    book.addOrder(order);

    EXPECT_FALSE(book.isEmpty());

    delete strategy;
}

TEST(OrderBookTest, AddSingleSellOrder) {
    auto strategy = new PriceTimeOrderMatchingStrategy();
    OrderBook book;
    book.setStrategy(strategy);

    auto order = std::make_shared<LimitOrder>(2, 101.0, 30, 101.0);
    order->setOrderType(OrderType::SELL);

    book.addOrder(order);

    EXPECT_FALSE(book.isEmpty());

    delete strategy;
}

TEST(OrderBookTest, MultipleOrdersSamePrice) {
    auto strategy = new PriceTimeOrderMatchingStrategy();
    OrderBook book;
    book.setStrategy(strategy);

    auto buy1 = std::make_shared<LimitOrder>(1, 100.0, 20, 100.0);
    auto buy2 = std::make_shared<LimitOrder>(2, 100.0, 30, 100.0);
    buy1->setOrderType(OrderType::BUY);
    buy2->setOrderType(OrderType::BUY);

    book.addOrder(buy1);
    book.addOrder(buy2);

    EXPECT_FALSE(book.isEmpty());

    // Sell order that partially matches the previous buy order
    auto sell = std::make_shared<LimitOrder>(3, 100.0, 25, 100.0);
    sell->setOrderType(OrderType::SELL);
    book.addOrder(sell);

    // After match: buy1 should be fully matched, buy2 should have 25-20 = 5 units matched
    EXPECT_FALSE(book.isEmpty());

    delete strategy;
}

TEST(OrderBookTest, MatchAllOrdersClearsBook) {
    auto strategy = new PriceTimeOrderMatchingStrategy();
    OrderBook book;
    book.setStrategy(strategy);

    auto buy1 = std::make_shared<LimitOrder>(1, 100.0, 50, 100.0);
    auto buy2 = std::make_shared<LimitOrder>(2, 100.0, 50, 100.0);
    buy1->setOrderType(OrderType::BUY);
    buy2->setOrderType(OrderType::BUY);
    book.addOrder(buy1);
    book.addOrder(buy2);

    auto sell = std::make_shared<LimitOrder>(3, 100.0, 100, 100.0);
    sell->setOrderType(OrderType::SELL);
    book.addOrder(sell);

    EXPECT_TRUE(book.isEmpty());

    delete strategy;
}

TEST(OrderBookTest, SellOrderPartiallyMatched) {
    auto strategy = new PriceTimeOrderMatchingStrategy();
    OrderBook book;
    book.setStrategy(strategy);

    auto buy = std::make_shared<LimitOrder>(1, 100.0, 40, 100.0);
    buy->setOrderType(OrderType::BUY);
    book.addOrder(buy);

    auto sell = std::make_shared<LimitOrder>(2, 100.0, 60, 100.0);
    sell->setOrderType(OrderType::SELL);
    book.addOrder(sell);

    EXPECT_FALSE(book.isEmpty());  // sell has 20 units remaining

    delete strategy;
}

TEST(OrderBookTest, MultipleLevelsOfBook) {
    auto strategy = new PriceTimeOrderMatchingStrategy();
    OrderBook book;
    book.setStrategy(strategy);

    auto buy1 = std::make_shared<LimitOrder>(1, 99.0, 20, 99.0);
    auto buy2 = std::make_shared<LimitOrder>(2, 98.0, 20, 98.0);
    auto sell1 = std::make_shared<LimitOrder>(3, 101.0, 20, 101.0);
    auto sell2 = std::make_shared<LimitOrder>(4, 102.0, 20, 102.0);
    buy1->setOrderType(OrderType::BUY);
    buy2->setOrderType(OrderType::BUY);
    sell1->setOrderType(OrderType::SELL);
    sell2->setOrderType(OrderType::SELL);

    book.addOrder(buy1);
    book.addOrder(buy2);
    book.addOrder(sell1);
    book.addOrder(sell2);

    EXPECT_FALSE(book.isEmpty());

    delete strategy;
}

TEST(OrderBookTest, NoMatchDueToSide) {
    auto strategy = new PriceTimeOrderMatchingStrategy();
    OrderBook book;
    book.setStrategy(strategy);

    auto buy1 = std::make_shared<LimitOrder>(1, 100.0, 20, 100.0);
    buy1->setOrderType(OrderType::BUY);
    auto buy2 = std::make_shared<LimitOrder>(2, 100.0, 30, 100.0);
    buy2->setOrderType(OrderType::BUY);

    book.addOrder(buy1);
    book.addOrder(buy2);

    // These are same side orders, no matching should happen
    EXPECT_FALSE(book.isEmpty());

    delete strategy;
}
