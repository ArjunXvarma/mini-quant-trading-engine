//
// Created by Arjun Varma on 14/07/25.
//

#ifndef ORDER_HPP
#define ORDER_HPP


#include <ctime>
#include <string>

#include "OrderType.hpp"

class Order {
public:
    Order(int traderId, double price, int quantity);
    virtual ~Order();
    virtual std::string getOrderClassType() const = 0;
    int getTraderId() const;
    double getPrice() const;
    int getQuantity() const;
    std::time_t getTimestamp() const;
    OrderType getOrderType() const;
    void setOrderType(OrderType);
    void reduceQuantity(int amount);
    int getOrderId() const;

private:
    int traderId;
    double price;
    int quantity;
    std::time_t timestamp;
    OrderType type;
    int orderId;

    static int orderCounter;
};



#endif //ORDER_HPP
