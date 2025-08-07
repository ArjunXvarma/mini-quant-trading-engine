#include "../../include/OrderMatchingStrategy/PriceTimeOrderMatchingStrategy.hpp"
#include "../../include/Utils/Logger.hpp"

#include <iomanip>
#include <sstream>

void matchBuyOrder(std::shared_ptr<Order> buyOrder, std::vector<std::shared_ptr<Order>>& sellOrders) {
    if (!buyOrder || buyOrder->getQuantity() <= 0) return;

    std::ostringstream oss;
    oss << "[MATCHING] Incoming BUY order: ID=" << buyOrder->getOrderId()
        << ", Price=" << buyOrder->getPrice()
        << ", Quantity=" << buyOrder->getQuantity();
    safeLog(oss.str());

    for (auto it = sellOrders.begin(); it != sellOrders.end() && buyOrder->getQuantity() > 0;) {
        auto sellOrder = *it;

        if (!sellOrder || sellOrder->getQuantity() <= 0) {
            it = sellOrders.erase(it);
            continue;
        }

        if (buyOrder->getPrice() >= sellOrder->getPrice()) {
            int tradedQty = std::min(buyOrder->getQuantity(), sellOrder->getQuantity());

            std::ostringstream matchLog;
            matchLog << "  > Match Found: SELL Order ID=" << sellOrder->getOrderId()
                     << " (Price=" << sellOrder->getPrice()
                     << ", Quantity=" << sellOrder->getQuantity() << ")"
                     << " --> Traded Quantity=" << tradedQty;
            safeLog(matchLog.str());

            buyOrder->reduceQuantity(tradedQty);
            sellOrder->reduceQuantity(tradedQty);

            if (sellOrder->getQuantity() == 0) {
                std::ostringstream removeLog;
                removeLog << "    - SELL Order ID=" << sellOrder->getOrderId()
                          << " fully filled and removed from book.";
                safeLog(removeLog.str());
                it = sellOrders.erase(it);
            } else {
                ++it;
            }
        } else {
            break;
        }
    }

    std::ostringstream resultLog;
    if (buyOrder->getQuantity() > 0) {
        resultLog << "  > BUY Order ID=" << buyOrder->getOrderId()
                  << " partially/unmatched. Remaining Quantity="
                  << buyOrder->getQuantity();
    } else {
        resultLog << "  > BUY Order ID=" << buyOrder->getOrderId() << " fully matched.";
    }
    safeLog(resultLog.str());
}

void matchSellOrder(
    std::shared_ptr<Order> sellOrder,
    std::vector<std::shared_ptr<Order>>& buyOrders
) {
    if (!sellOrder || sellOrder->getQuantity() <= 0) return;

    std::ostringstream oss;
    oss << "[MATCHING] Incoming SELL order: ID=" << sellOrder->getOrderId()
        << ", Price=" << sellOrder->getPrice()
        << ", Quantity=" << sellOrder->getQuantity();
    safeLog(oss.str());

    for (auto it = buyOrders.begin(); it != buyOrders.end() && sellOrder->getQuantity() > 0;) {
        auto buyOrder = *it;

        if (!buyOrder || buyOrder->getQuantity() <= 0) {
            it = buyOrders.erase(it);
            continue;
        }

        if (sellOrder->getPrice() <= buyOrder->getPrice()) {
            int tradedQty = std::min(sellOrder->getQuantity(), buyOrder->getQuantity());

            std::ostringstream matchLog;
            matchLog << "  > Match Found: BUY Order ID=" << buyOrder->getOrderId()
                     << " (Price=" << buyOrder->getPrice()
                     << ", Quantity=" << buyOrder->getQuantity() << ")"
                     << " --> Traded Quantity=" << tradedQty;
            safeLog(matchLog.str());

            buyOrder->reduceQuantity(tradedQty);
            sellOrder->reduceQuantity(tradedQty);

            if (buyOrder->getQuantity() == 0) {
                std::ostringstream removeLog;
                removeLog << "    - BUY Order ID=" << buyOrder->getOrderId()
                          << " fully filled and removed from book.";
                safeLog(removeLog.str());
                it = buyOrders.erase(it);
            } else {
                ++it;
            }
        } else {
            break;
        }
    }

    std::ostringstream resultLog;
    if (sellOrder->getQuantity() > 0) {
        resultLog << "  > SELL Order ID=" << sellOrder->getOrderId()
                  << " partially/unmatched. Remaining Quantity="
                  << sellOrder->getQuantity();
    } else {
        resultLog << "  > SELL Order ID=" << sellOrder->getOrderId() << " fully matched.";
    }
    safeLog(resultLog.str());
}

void PriceTimeOrderMatchingStrategy::matchOrders(
    std::vector<std::shared_ptr<Order>>& restingBuyOrders,
    std::vector<std::shared_ptr<Order>>& restingSellOrders,
    std::shared_ptr<Order> incomingOrder
) {
    if (!incomingOrder) return;

    safeLog("\n=============================");

    std::ostringstream orderLog;
    orderLog << ">> New Incoming Order: ID=" << incomingOrder->getOrderId()
             << ", Type=" << (incomingOrder->getOrderType() == OrderType::BUY ? "BUY" : "SELL")
             << ", Price=" << incomingOrder->getPrice()
             << ", Quantity=" << incomingOrder->getQuantity();
    safeLog(orderLog.str());

    if (incomingOrder->getQuantity() <= 0) {
        safeLog(">> Skipping order with non-positive quantity.");
        safeLog("=============================\n");
        return;
    }

    if (incomingOrder->getOrderType() == OrderType::BUY) {
        matchBuyOrder(incomingOrder, restingSellOrders);
        if (incomingOrder->getQuantity() > 0) {
            restingBuyOrders.push_back(incomingOrder);
            std::ostringstream addLog;
            addLog << ">> BUY Order ID=" << incomingOrder->getOrderId() << " added to order book.";
            safeLog(addLog.str());
        }
    } else {
        matchSellOrder(incomingOrder, restingBuyOrders);
        if (incomingOrder->getQuantity() > 0) {
            restingSellOrders.push_back(incomingOrder);
            std::ostringstream addLog;
            addLog << ">> SELL Order ID=" << incomingOrder->getOrderId() << " added to order book.";
            safeLog(addLog.str());
        }
    }

    safeLog("=============================\n");
}
