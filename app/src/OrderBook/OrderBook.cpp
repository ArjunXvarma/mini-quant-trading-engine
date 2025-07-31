#include "../../include/OrderBook/OrderBook.hpp"
#include "../../include/Utils/Logger.hpp"

#include <algorithm>
#include <sstream>

OrderBook::OrderBook() = default;

void OrderBook::setStrategy(OrderMatchingStrategy* strat) {
    strategy = strat;
}

void OrderBook::sortOrders() {
    std::sort(buyOrders.begin(), buyOrders.end(), [](const auto& a, const auto& b) {
        return a->getPrice() > b->getPrice() ||
              (a->getPrice() == b->getPrice() && a->getTimestamp() < b->getTimestamp());
    });

    std::sort(sellOrders.begin(), sellOrders.end(), [](const auto& a, const auto& b) {
        return a->getPrice() < b->getPrice() ||
              (a->getPrice() == b->getPrice() && a->getTimestamp() < b->getTimestamp());
    });
}

void OrderBook::addOrder(const std::shared_ptr<Order>& order) {
    std::lock_guard<std::mutex> lock(mutex);

    if (!strategy) {
        safeLog("[OrderBook] Error: No matching strategy set!");
        return;
    }

    std::ostringstream oss;
    oss << "[OrderBook] Adding " << (order->getOrderType() == OrderType::BUY ? "BUY" : "SELL")
        << " Order | Price: " << order->getPrice()
        << " | Qty: " << order->getQuantity()
        << " | Trader ID: " << order->getTraderId();
    safeLog(oss.str());

    matchOrder(order);
}

void OrderBook::matchOrder(const std::shared_ptr<Order>& order) {
    if (!strategy) {
        safeLog("[OrderBook] Error: No matching strategy set!");
        return;
    }

    strategy->matchOrders(buyOrders, sellOrders, order);
}

void OrderBook::printOrders() const {
    std::lock_guard<std::mutex> lock(mutex);

    std::ostringstream oss;
    if (buyOrders.empty() && sellOrders.empty()) {
        oss << "Order book is empty.\n";
        safeLog(oss.str());
        return;
    }

    oss << "================== ORDER BOOK ==================\n";
    oss << "ID\tPrice\tQty\tType\n";

    for (const auto& order : buyOrders) {
        oss << order->getOrderId() << "\t" << order->getPrice() << "\t"
            << order->getQuantity() << "\tBUY\n";
    }

    for (const auto& order : sellOrders) {
        oss << order->getOrderId() << "\t" << order->getPrice() << "\t"
            << order->getQuantity() << "\tSELL\n";
    }

    oss << "===============================================\n";
    safeLog(oss.str());
}

std::vector<std::shared_ptr<Order>> OrderBook::getAllOrders() const {
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<std::shared_ptr<Order>> orders;
    orders.reserve(buyOrders.size() + sellOrders.size());

    for (const auto& order : buyOrders)
        orders.push_back(order);

    for (const auto& order : sellOrders)
        orders.push_back(order);

    return orders;
}

bool OrderBook::isEmpty() const {
    return buyOrders.empty() && sellOrders.empty();
}