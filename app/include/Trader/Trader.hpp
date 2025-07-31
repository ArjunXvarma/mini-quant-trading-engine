//
// Created by Arjun Varma on 14/07/25.
//

#ifndef TRADER_HPP
#define TRADER_HPP


#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "../MarketDataHandler/Stock.hpp"
#include "../Orders/OrderFactory.hpp"
#include "../OrderBook/OrderBook.hpp"
#include "../Orders/Order.hpp"

class TraderException : public std::exception {
private:
    std::string message;

public:
    TraderException(const std::string& message);
    virtual const char* what() const noexcept override;
};


class Trader {
public:
    Trader();
    Trader(const std::string&);
    Trader(const std::string&, const Stock&, OrderFactory*, OrderBook*);
    
    void buy(Stock& stock, int quantity);
    void sell(Stock& stock, int quantity);

    void trade(double, double, int, int);
    int getID() const;
    const std::vector<std::shared_ptr<Order>>& getOrders() const;
    Stock getStock() const;
    
    ~Trader();

private:
    static int id_counter;
    int id;
    std::string name;
    Stock stock;
    OrderFactory* orderFactory;
    OrderBook* orderBook;
    std::vector<std::shared_ptr<Order>> orders;
};


#endif //TRADER_HPP
