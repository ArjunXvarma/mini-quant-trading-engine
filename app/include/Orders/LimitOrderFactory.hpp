//
// Created by Arjun Varma on 14/07/25.
//

#ifndef LIMITORDERFACTORY_HPP
#define LIMITORDERFACTORY_HPP


#include "OrderFactory.hpp"
#include "LimitOrder.hpp"

class LimitOrderFactory : public OrderFactory {
public:
    std::shared_ptr<Order> createOrder(int traderId, double price, int quantity) override;
};


#endif //LIMITORDERFACTORY_HPP
