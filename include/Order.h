#pragma once

#include <cstdint>

enum class Side
{
    BUY,
    SELL
};

struct Order
{
    uint64_t id;
    Side side;
    int64_t price;
    int64_t quantity;
    uint64_t timestamp;
};