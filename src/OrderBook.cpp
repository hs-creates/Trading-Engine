
#include "OrderBook.h"

#include <algorithm>
#include <stdexcept>

OrderBook::OrderBook() = default;


// ADD ORDER
std::vector<Trade> OrderBook::addOrder(const Order& order)
{
    // Validate incoming order
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

    // Reject duplicate active order IDs
    if (orderIndex.find(order.id) != orderIndex.end())
    {
        throw std::invalid_argument("Duplicate active order ID");
    }

    Order incoming = order;

    std::vector<Trade> trades;


    // BUY ORDER: Match against existing SELL orders
    if (incoming.side == Side::BUY)
    {
        while (incoming.quantity > 0 && !asks.empty())
        {
            auto bestAsk = asks.begin();

            // No compatible selling price
            if (bestAsk->first > incoming.price)
            {
                break;
            }

            auto& level = bestAsk->second;

            // Match against oldest order at this price
            while (incoming.quantity > 0 && !level.empty())
            {
                Order& restingOrder = level.front();

                int64_t tradedQuantity =
                    std::min(incoming.quantity, restingOrder.quantity);

                // Execute trade at resting order's price
                trades.push_back({
                    incoming.id,
                    restingOrder.id,
                    restingOrder.price,
                    tradedQuantity
                });

                incoming.quantity -= tradedQuantity;
                restingOrder.quantity -= tradedQuantity;

                // Remove fully filled resting order
                if (restingOrder.quantity == 0)
                {
                    orderIndex.erase(restingOrder.id);
                    level.pop_front();
                }
            }

            // Remove empty price level
            if (level.empty())
            {
                asks.erase(bestAsk);
            }
        }

        // Add any remaining quantity to the BUY side
        if (incoming.quantity > 0)
        {
            auto& level = bids[incoming.price];

            level.push_back(incoming);

            auto inserted = std::prev(level.end());

            orderIndex.emplace(
                incoming.id,
                OrderLocation{
                    incoming.price,
                    true,
                    inserted
                }
            );
        }
    }


    // SELL ORDER: Match against existing BUY orders
    else
    {
        while (incoming.quantity > 0 && !bids.empty())
        {
            auto bestBid = bids.begin();

            // No compatible buying price
            if (bestBid->first < incoming.price)
            {
                break;
            }

            auto& level = bestBid->second;

            // Match against oldest order at this price
            while (incoming.quantity > 0 && !level.empty())
            {
                Order& restingOrder = level.front();

                int64_t tradedQuantity =
                    std::min(incoming.quantity, restingOrder.quantity);

                // Execute trade at resting order's price
                trades.push_back({
                    restingOrder.id,
                    incoming.id,
                    restingOrder.price,
                    tradedQuantity
                });

                incoming.quantity -= tradedQuantity;
                restingOrder.quantity -= tradedQuantity;

                // Remove fully filled resting order
                if (restingOrder.quantity == 0)
                {
                    orderIndex.erase(restingOrder.id);
                    level.pop_front();
                }
            }

            // Remove empty price level
            if (level.empty())
            {
                bids.erase(bestBid);
            }
        }

        // Add any remaining quantity to the SELL side
        if (incoming.quantity > 0)
        {
            auto& level = asks[incoming.price];

            level.push_back(incoming);

            auto inserted = std::prev(level.end());

            orderIndex.emplace(
                incoming.id,
                OrderLocation{
                    incoming.price,
                    false,
                    inserted
                }
            );
        }
    }

    return trades;
}


// CANCEL ORDER
bool OrderBook::cancelOrder(uint64_t orderId)
{
    auto indexIt = orderIndex.find(orderId);

    // Order does not exist
    if (indexIt == orderIndex.end())
    {
        return false;
    }

    OrderLocation location = indexIt->second;

    // Cancel BUY order
    if (location.isBuy)
    {
        auto levelIt = bids.find(location.price);

        if (levelIt == bids.end())
        {
            return false;
        }

        // Erase directly using the stored iterator
        levelIt->second.erase(location.iterator);

        orderIndex.erase(indexIt);

        // Remove empty price level
        if (levelIt->second.empty())
        {
            bids.erase(levelIt);
        }
    }

    // Cancel SELL order
    else
    {
        auto levelIt = asks.find(location.price);

        if (levelIt == asks.end())
        {
            return false;
        }

        // Erase directly using the stored iterator
        levelIt->second.erase(location.iterator);

        orderIndex.erase(indexIt);

        // Remove empty price level
        if (levelIt->second.empty())
        {
            asks.erase(levelIt);
        }
    }

    return true;
}


// GET ORDER
std::optional<Order> OrderBook::getOrder(uint64_t orderId) const
{
    auto it = orderIndex.find(orderId);

    if (it == orderIndex.end())
    {
        return std::nullopt;
    }

    return *(it->second.iterator);
}


// GET TOTAL ACTIVE ORDER COUNT
std::size_t OrderBook::getOrderCount() const
{
    return orderIndex.size();
}
