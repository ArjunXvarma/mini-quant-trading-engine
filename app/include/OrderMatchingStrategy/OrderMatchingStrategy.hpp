//
// Created by Arjun Varma on 14/07/25.
//

#ifndef ORDERMATCHINGSTRATEGY_HPP
#define ORDERMATCHINGSTRATEGY_HPP

#include <memory>
#include <vector>

#include "../Orders/Order.hpp"
#include "../include/Utils/Logger.hpp"


class OrderMatchingStrategy {
public:
    virtual void matchOrders(std::vector<std::shared_ptr<Order>>& restingBuyOrders, std::vector<std::shared_ptr<Order>>& restingSellOrders, std::shared_ptr<Order> incomingOrder) = 0;
    virtual ~OrderMatchingStrategy();
};


#endif //ORDERMATCHINGSTRATEGY_HPP
