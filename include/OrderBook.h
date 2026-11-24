#pragma once

#include "Order.h"
#include <map>
#include <deque>
#include <functional>

class OrderBook
{
private:
    std::map<int64_t, std::deque<Order>,
             std::greater<int64_t>> bids;

    std::map<int64_t, std::deque<Order>> asks;

public:
    void addOrder(const Order& order);
};