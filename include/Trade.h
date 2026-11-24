#pragma once

#include <cstdint>

struct Trade
{
    uint64_t buyOrderId;
    uint64_t sellOrderId;

    int64_t price;
    int64_t quantity;
};