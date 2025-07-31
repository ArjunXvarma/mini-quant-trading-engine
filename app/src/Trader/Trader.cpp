//
// Created by Arjun Varma on 14/07/25.
//

#include "../../include/Trader/Trader.hpp"

TraderException::TraderException(const std::string& mess)
		: message(mess) {

}

const char* TraderException::what() const noexcept {
	return message.c_str();
}

int Trader::id_counter = 0;

Trader::Trader()
	: name{""}, id{id_counter++}, orderFactory{nullptr}, orderBook{nullptr}, stock{} {

}

Trader::Trader(const std::string& name)
    	: name{name}, orderBook{nullptr}, orderFactory{nullptr}, id{id_counter++}, stock{} {

}

Trader::Trader(const std::string& n, const Stock& s, OrderFactory* o, OrderBook* b)
    	: id{id_counter++}, name{n}, stock{s}, orderFactory{o}, orderBook{b} {

}

void Trader::buy(Stock& stock, int quantity) {
    try {
        if (orderFactory != nullptr && orderBook != nullptr) {
            	std::shared_ptr<Order> ord = orderFactory->createOrder(id, stock.getPrice(), quantity);
            	orders.push_back(ord);
            	if (ord != nullptr) {
                	orderBook->addOrder(ord);
            	}
        }
    }
    catch (const TraderException& e) {
        // Handle trader-specific exceptions
        std::cerr << "Trader Exception: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        // Handle other exceptions (if any)
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void Trader::sell(Stock& stock, int quantity) {
    try {
        if (orderFactory != nullptr && orderBook != nullptr) {
            	std::shared_ptr<Order> ord = orderFactory->createOrder(id, -stock.getPrice(), quantity);
            	orders.push_back(ord);
            	if (ord != nullptr) {
                	orderBook->addOrder(ord);
            	}
        }
    }
    catch (const TraderException& e) {
        // Handle trader-specific exceptions
        std::cerr << "Trader Exception: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        // Handle other exceptions (if any)
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int Trader::getID() const {
	return id;
}

const std::vector<std::shared_ptr<Order>>& Trader::getOrders() const {
    	return orders;
}

Stock Trader::getStock() const {
    	return stock;
}

Trader::~Trader() {
    delete orderFactory;
    delete orderBook;
}