
#include "OrderBook.h"

#include <iostream>

int main()
{
    OrderBook book;

    // Add three buy orders
    book.addOrder({1, Side::BUY, 15000, 100, 1});
    book.addOrder({2, Side::BUY, 15000, 50, 2});
    book.addOrder({3, Side::BUY, 14900, 75, 3});

    std::cout << "Active orders: "
              << book.getOrderCount() << '\n';

    // Retrieve order 2
    auto order = book.getOrder(2);

    if (order.has_value())
    {
        std::cout << "Found order: "
                  << order->id << '\n';

        std::cout << "Price: "
                  << order->price << '\n';

        std::cout << "Quantity: "
                  << order->quantity << '\n';
    }

    // Cancel order 2
    bool cancelled = book.cancelOrder(2);

    std::cout << "Cancellation successful: "
              << std::boolalpha << cancelled << '\n';

    std::cout << "Active orders after cancellation: "
              << book.getOrderCount() << '\n';

    // Try cancelling the same order again
    bool cancelledAgain = book.cancelOrder(2);

    std::cout << "Second cancellation: "
              << cancelledAgain << '\n';

    return 0;
}
