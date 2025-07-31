//
// Created by Arjun Varma on 14/07/25.
//

#include "../../include/Orders/Order.hpp"

int Order::orderCounter = 0;

Order::Order(int traderId, double price, int quantity) : traderId(traderId), price(price), quantity(quantity) {
    orderId = ++orderCounter;
}

Order::~Order() {

}

int Order::getTraderId() const {
    return traderId;
}

double Order::getPrice() const {
    return price;
}

int Order::getQuantity() const {
    return quantity;
}

std::time_t Order::getTimestamp() const {
    return timestamp;
}

void Order::setOrderType(OrderType type) {
    this->type = type;
}

OrderType Order::getOrderType() const {
    return type;
}

int Order::getOrderId() const {
    return orderId;
}

void Order::reduceQuantity(int amount) {
    if (amount > 0 && amount <= quantity)
        quantity -= amount;

    else
        throw std::out_of_range{"Amount out of range."};
}


