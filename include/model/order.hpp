#pragma once

#include "book/order.hpp"
#include "book/types.hpp"

namespace market { 
    namespace model { 

        enum OrderState {
            os_new,
            os_accepted,
            os_complete,
            os_cancelled,
            os_rejected
        };

        class Order {
            public:
                Order();

                void initialize(
                    bool is_buy,
                    market::book::Price price,
                    market::book::Quantity qty
                );

                /// @brief get the order's state
                const OrderState& state() const;

                /// @brief is this order a buy?
                virtual bool is_buy() const;

                /// @brief is this a limit order?
                virtual bool is_limit() const;

                /// @brief get the limit price of this order
                virtual market::book::Price price() const;

                /// @brief get the quantity of this order
                virtual market::book::Quantity order_qty() const;

                /// @brief get the open quantity of this order
                virtual market::book::Quantity open_qty() const;

                /// @brief get the filled quantity of this order
                virtual const market::book::Quantity& filled_qty() const;

                /// @brief get the total filled cost of this order
                const market::book::Cost& filled_cost() const;

                /// @brief notify of a fill of this order
                /// @param fill_qty the number of shares in this fill
                /// @param fill_cost the total amount of this fill
                /// @fill_id the unique identifier of this fill
                virtual void fill(
                    market::book::Quantity fill_qty,
                    market::book::Cost fill_cost,
                    market::book::FillId fill_id
                );

                /// @brief exchange accepted this order
                void accept();

                /// @brief exchange cancelled this order
                void cancel();

                /// @brief exchange replaced this order
                /// @param size_delta change to the order quantity
                /// @param new_price the new price
                void replace(
                    market::book::Quantity size_delta,
                    market::book::Price new_price
                );

            private:
                OrderState state_;
                bool is_buy_;
                book::Price    price_;
                book::Quantity order_qty_;
                book::Quantity filled_qty_;
                book::Cost filled_cost_;

            public:
                const int64_t order_id_; // Just a sequence number!

        };
    } // namespace model
} // namespace market
