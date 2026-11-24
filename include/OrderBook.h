
#pragma once

#include "Order.h"
#include "Trade.h"

#include <map>
#include <deque>
#include <functional>
#include <vector>

class OrderBook
{
private:
    // Highest bid first
    std::map<int64_t, std::deque<Order>,
             std::greater<int64_t>> bids;

    // Lowest ask first
    std::map<int64_t, std::deque<Order>> asks;

public:
    OrderBook();

    std::vector<Trade> addOrder(const Order& order);
};
