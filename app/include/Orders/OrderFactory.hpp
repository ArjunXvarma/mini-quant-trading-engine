//
// Created by Arjun Varma on 14/07/25.
//

#ifndef ORDERFACTORY_HPP
#define ORDERFACTORY_HPP


#include <memory>

#include "Order.hpp"
#include "OrderType.hpp"

class OrderFactory {
public:
    virtual std::shared_ptr<Order> createOrder(int traderId, double price, int quantity) = 0;
    virtual ~OrderFactory() {}
};

#endif //ORDERFACTORY_HPP
