//
// Created by Arjun Varma on 14/07/25.
//

#include "../../include/Orders/MarketOrderFactory.hpp"

std::shared_ptr<Order> MarketOrderFactory::createOrder(int traderId, double price, int quantity) {
    return std::make_unique<MarketOrder>(traderId, price, quantity);
}