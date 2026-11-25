#pragma once

#include "Order.h"
#include "Trade.h"
#include "OrderLocation.h"

#include <map>
#include <list>
#include <functional>
#include <vector>
#include <unordered_map>
#include <optional>
#include <cstdint>

class OrderBook {
private:
    std::map<int64_t, std::list<Order>,
             std::greater<int64_t>> bids;

    std::map<int64_t, std::list<Order>> asks;

    std::unordered_map<uint64_t, OrderLocation> orderIndex;

public:
    OrderBook();

    std::vector<Trade> addOrder(const Order& order);

    bool cancelOrder(uint64_t orderId);

    std::optional<Order> getOrder(uint64_t orderId) const;

    std::size_t getOrderCount() const;
};