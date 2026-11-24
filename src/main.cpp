
#include "OrderBook.h"

#include <iostream>

int main()
{
    OrderBook book;

    // Add a sell order:
    // Order ID 1, £150.00, quantity 100
    Order sellOrder{
        1,
        Side::SELL,
        15000,
        100,
        1
    };

    book.addOrder(sellOrder);

    // Add a buy order:
    // Order ID 2, £151.00, quantity 40
    Order buyOrder{
        2,
        Side::BUY,
        15100,
        40,
        2
    };

    std::vector<Trade> trades = book.addOrder(buyOrder);

    // Display executed trades
    for (const Trade& trade : trades)
    {
        std::cout << "Trade executed!\n";

        std::cout << "Buy Order ID: "
                  << trade.buyOrderId << '\n';

        std::cout << "Sell Order ID: "
                  << trade.sellOrderId << '\n';

        std::cout << "Price: "
                  << trade.price << '\n';

        std::cout << "Quantity: "
                  << trade.quantity << '\n';

        std::cout << "-------------------\n";
    }

    return 0;
}
