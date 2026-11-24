#include "OrderBook.h"

#include <iostream>

int main()
{
    OrderBook orderBook;

    Order buyOrder
    {
        1,
        Side::BUY,
        15000,
        100,
        1
    };

    Order sellOrder
    {
        2,
        Side::SELL,
        15100,
        50,
        2
    };

    orderBook.addOrder(buyOrder);
    orderBook.addOrder(sellOrder);

    std::cout << "Trading engine initialised.\n";
    std::cout << "Orders successfully submitted.\n";

    return 0;
}