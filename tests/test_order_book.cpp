
#include <gtest/gtest.h>

#include "OrderBook.h"

#include <stdexcept>

TEST(OrderBookTests, AddsAndRetrievesBuyOrder)
{
    OrderBook book;

    Order order{
        1,
        Side::BUY,
        15000,
        100,
        1
    };

    book.addOrder(order);

    EXPECT_EQ(book.getOrderCount(), 1);

    auto result = book.getOrder(1);

    ASSERT_TRUE(result.has_value());

    EXPECT_EQ(result->id, 1);
    EXPECT_EQ(result->price, 15000);
    EXPECT_EQ(result->quantity, 100);
    EXPECT_EQ(result->side, Side::BUY);
}

TEST(OrderBookTests, MatchesCompatibleBuyAndSellOrders)
{
    OrderBook book;

    // Seller offers 100 shares at £150
    book.addOrder({
        1,
        Side::SELL,
        15000,
        100,
        1
    });

    // Buyer wants 40 shares and is willing to pay £151
    std::vector<Trade> trades = book.addOrder({
        2,
        Side::BUY,
        15100,
        40,
        2
    });

    ASSERT_EQ(trades.size(), 1);

    EXPECT_EQ(trades[0].buyOrderId, 2);
    EXPECT_EQ(trades[0].sellOrderId, 1);
    EXPECT_EQ(trades[0].price, 15000);
    EXPECT_EQ(trades[0].quantity, 40);

    EXPECT_EQ(book.getOrderCount(), 1);

    auto remainingOrder = book.getOrder(1);

    ASSERT_TRUE(remainingOrder.has_value());

    EXPECT_EQ(remainingOrder->quantity, 60);
}

TEST(OrderBookTests, MatchesBestSellPriceFirst)
{
    OrderBook book;

    // Seller A: higher price
    book.addOrder({
        1,
        Side::SELL,
        15200,
        50,
        1
    });

    // Seller B: lower price
    book.addOrder({
        2,
        Side::SELL,
        15000,
        50,
        2
    });

    // Buyer wants 50 shares
    std::vector<Trade> trades = book.addOrder({
        3,
        Side::BUY,
        15500,
        50,
        3
    });

    ASSERT_EQ(trades.size(), 1);

    EXPECT_EQ(trades[0].sellOrderId, 2);
    EXPECT_EQ(trades[0].price, 15000);

    EXPECT_TRUE(book.getOrder(1).has_value());
    EXPECT_FALSE(book.getOrder(2).has_value());
}

TEST(OrderBookTests, MatchesEarlierOrderFirstAtSamePrice)
{
    OrderBook book;

    book.addOrder({
        1,
        Side::SELL,
        15000,
        30,
        1
    });

    book.addOrder({
        2,
        Side::SELL,
        15000,
        50,
        2
    });

    std::vector<Trade> trades = book.addOrder({
        3,
        Side::BUY,
        15000,
        40,
        3
    });

    ASSERT_EQ(trades.size(), 2);

    EXPECT_EQ(trades[0].sellOrderId, 1);
    EXPECT_EQ(trades[0].quantity, 30);

    EXPECT_EQ(trades[1].sellOrderId, 2);
    EXPECT_EQ(trades[1].quantity, 10);

    auto remainingOrder = book.getOrder(2);

    ASSERT_TRUE(remainingOrder.has_value());

    EXPECT_EQ(remainingOrder->quantity, 40);

    EXPECT_FALSE(book.getOrder(1).has_value());
}

TEST(OrderBookTests, CancelsOrderAndUpdatesIndex)
{
    OrderBook book;

    book.addOrder({1, Side::BUY, 15000, 100, 1});
    book.addOrder({2, Side::BUY, 15000, 100, 2});
    book.addOrder({3, Side::BUY, 15000, 100, 3});

    EXPECT_EQ(book.getOrderCount(), 3);

    EXPECT_TRUE(book.cancelOrder(2));

    EXPECT_EQ(book.getOrderCount(), 2);

    EXPECT_FALSE(book.getOrder(2).has_value());

    // Order 3 should still be accessible
    auto thirdOrder = book.getOrder(3);

    ASSERT_TRUE(thirdOrder.has_value());

    EXPECT_EQ(thirdOrder->id, 3);
    EXPECT_EQ(thirdOrder->quantity, 100);
}

TEST(OrderBookTests, ReturnsFalseWhenCancellingUnknownOrder)
{
    OrderBook book;

    EXPECT_FALSE(book.cancelOrder(999));
}

TEST(OrderBookTests, RejectsDuplicateActiveOrderIDs)
{
    OrderBook book;

    book.addOrder({
        1,
        Side::BUY,
        15000,
        100,
        1
    });

    EXPECT_THROW(
        book.addOrder({
            1,
            Side::SELL,
            15100,
            50,
            2
        }),
        std::invalid_argument
    );

    EXPECT_EQ(book.getOrderCount(), 1);
}

TEST(OrderBookTests, RejectsInvalidOrders)
{
    OrderBook book;

    EXPECT_THROW(
        book.addOrder({
            1,
            Side::BUY,
            15000,
            0,
            1
        }),
        std::invalid_argument
    );

    EXPECT_THROW(
        book.addOrder({
            2,
            Side::SELL,
            -100,
            50,
            2
        }),
        std::invalid_argument
    );

    EXPECT_THROW(
        book.addOrder({
            0,
            Side::BUY,
            15000,
            50,
            3
        }),
        std::invalid_argument
    );

    EXPECT_EQ(book.getOrderCount(), 0);
}

TEST(OrderBookTests, DoesNotMatchOrdersWithIncompatiblePrices)
{
    OrderBook book;

    book.addOrder({
        1,
        Side::SELL,
        15000,
        100,
        1
    });

    std::vector<Trade> trades = book.addOrder({
        2,
        Side::BUY,
        14900,
        100,
        2
    });

    EXPECT_TRUE(trades.empty());

    EXPECT_EQ(book.getOrderCount(), 2);

    EXPECT_TRUE(book.getOrder(1).has_value());
    EXPECT_TRUE(book.getOrder(2).has_value());
}
