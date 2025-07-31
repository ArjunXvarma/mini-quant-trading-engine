//
// Created by Arjun Varma on 14/07/25.
//

#ifndef MARKETORDER_HPP
#define MARKETORDER_HPP


#include "Order.hpp"

class MarketOrder : public Order {
public:
    MarketOrder(int traderId, double price, int quantity) : Order(traderId, price, quantity) {}
    std::string getOrderClassType() const override;
};



#endif //MARKETORDER_HPP
