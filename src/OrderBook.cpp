
#include "OrderBook.h"

#include <algorithm>
#include <stdexcept>

OrderBook::OrderBook()
{
}

std::vector<Trade> OrderBook::addOrder(const Order& order)
{
    // Validate the incoming order
    if (order.id == 0)
    {
        throw std::invalid_argument("Order ID cannot be zero");
    }

    if (order.price <= 0)
    {
        throw std::invalid_argument("Order price must be positive");
    }

    if (order.quantity <= 0)
    {
        throw std::invalid_argument("Order quantity must be positive");
    }

    // Make a copy because we need to modify its remaining quantity
    Order incoming = order;

    std::vector<Trade> trades;

    // BUY order: match against the cheapest available SELL orders
    if (incoming.side == Side::BUY)
    {
        while (incoming.quantity > 0 &&
               !asks.empty() &&
               asks.begin()->first <= incoming.price)
        {
            auto bestAsk = asks.begin();

            Order& restingOrder = bestAsk->second.front();

            int64_t tradedQuantity =
                std::min(incoming.quantity, restingOrder.quantity);

            Trade trade;

            trade.buyOrderId = incoming.id;
            trade.sellOrderId = restingOrder.id;
            trade.price = restingOrder.price;
            trade.quantity = tradedQuantity;

            trades.push_back(trade);

            incoming.quantity -= tradedQuantity;
            restingOrder.quantity -= tradedQuantity;

            // Remove a fully filled resting order
            if (restingOrder.quantity == 0)
            {
                bestAsk->second.pop_front();
            }

            // Remove an empty price level
            if (bestAsk->second.empty())
            {
                asks.erase(bestAsk);
            }
        }

        // Add any unfilled quantity to the bid book
        if (incoming.quantity > 0)
        {
            bids[incoming.price].push_back(incoming);
        }
    }

    // SELL order: match against the highest available BUY orders
    else
    {
        while (incoming.quantity > 0 &&
               !bids.empty() &&
               bids.begin()->first >= incoming.price)
        {
            auto bestBid = bids.begin();

            Order& restingOrder = bestBid->second.front();

            int64_t tradedQuantity =
                std::min(incoming.quantity, restingOrder.quantity);

            Trade trade;

            trade.buyOrderId = restingOrder.id;
            trade.sellOrderId = incoming.id;
            trade.price = restingOrder.price;
            trade.quantity = tradedQuantity;

            trades.push_back(trade);

            incoming.quantity -= tradedQuantity;
            restingOrder.quantity -= tradedQuantity;

            // Remove a fully filled resting order
            if (restingOrder.quantity == 0)
            {
                bestBid->second.pop_front();
            }

            // Remove an empty price level
            if (bestBid->second.empty())
            {
                bids.erase(bestBid);
            }
        }

        // Add any unfilled quantity to the ask book
        if (incoming.quantity > 0)
        {
            asks[incoming.price].push_back(incoming);
        }
    }

    return trades;
}
