//
// Created by Arjun Varma on 14/07/25.
//

#ifndef LIMITORDER_HPP
#define LIMITORDER_HPP


#include "Order.hpp"

class LimitOrder : public Order {
private:
    double limit;
public:
    LimitOrder(int traderId, double price, int quantity, double limit) : Order(traderId, price, quantity), limit(limit) {}
    std::string getOrderClassType() const override;

    double getLimit() const { return limit; }
    void setLimit(double limit) { this->limit = limit; }
};




#endif //LIMITORDER_HPP
