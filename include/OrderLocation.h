#pragma once

#include <cstdint>
#include <list>
#include "Order.h"

struct OrderLocation {
    int64_t price;
    bool isBuy;
    std::list<Order>::iterator iterator;
};