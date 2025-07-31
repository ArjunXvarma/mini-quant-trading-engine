//
// Created by Arjun Varma on 14/07/25.
//

#include "../../include/Orders/LimitOrderFactory.hpp"

std::shared_ptr<Order> LimitOrderFactory::createOrder(int traderId, double price, int quantity) {
    return std::make_unique<LimitOrder>(traderId, price, quantity, 0);
}