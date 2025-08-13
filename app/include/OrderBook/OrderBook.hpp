//
// Created by Arjun Varma on 14/07/25.
//

#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <vector>
#include <memory>
#include <mutex>
#include <chrono>

#include "../Orders/Order.hpp"
#include "../OrderMatchingStrategy/OrderMatchingStrategy.hpp"

class OrderBook {
private:
    std::vector<std::shared_ptr<Order>> buyOrders;
    std::vector<std::shared_ptr<Order>> sellOrders;
    OrderMatchingStrategy* strategy = nullptr;
    mutable std::mutex mutex;

    void sortOrders();

public:
    OrderBook();

    void setStrategy(OrderMatchingStrategy* strat);

    void addOrder(const std::shared_ptr<Order>& order);
    void matchOrder(const std::shared_ptr<Order>& order);

    void printOrders() const;
    std::vector<std::shared_ptr<Order>> getAllOrders() const;

    bool isEmpty() const;
};

#endif // ORDERBOOK_HPP
