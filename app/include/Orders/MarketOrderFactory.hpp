//
// Created by Arjun Varma on 14/07/25.
//

#ifndef MARKETORDERFACTORY_HPP
#define MARKETORDERFACTORY_HPP


#include "OrderFactory.hpp"
#include "MarketOrder.hpp"

class MarketOrderFactory : public OrderFactory {
public:
    std::shared_ptr<Order> createOrder(int traderId, double price, int quantity) override;
};


#endif //MARKETORDERFACTORY_HPP
