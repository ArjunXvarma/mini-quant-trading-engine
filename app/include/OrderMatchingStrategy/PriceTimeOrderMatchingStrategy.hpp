//
// Created by Arjun Varma on 14/07/25.
//

#ifndef PRICETIMEORDERMATCHINGSTRATEGY_HPP
#define PRICETIMEORDERMATCHINGSTRATEGY_HPP

#include "OrderMatchingStrategy.hpp"
#include "../Orders/Order.hpp"

class PriceTimeOrderMatchingStrategy : public OrderMatchingStrategy {
public:
    void matchOrders(std::vector<std::shared_ptr<Order>>& restingBuyOrders, std::vector<std::shared_ptr<Order>>& restingSellOrders, std::shared_ptr<Order> incomingOrder) override;
};


#endif //PRICETIMEORDERMATCHINGSTRATEGY_HPP
