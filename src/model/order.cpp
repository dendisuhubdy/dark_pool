#include "model/order.hpp"

#include <iostream>

namespace market { 
    namespace model { 

        Order::Order(
        ) : state_(os_new),
            is_buy_(0),
            price_(0),
            order_qty_(0),   
            filled_qty_(0),
            filled_cost_(0),
            order_id_(0) 
        {
        }

        void Order::initialize(
            bool is_buy,
            book::Price price,
            book::Quantity qty
        ) 
        {
            is_buy_    = is_buy;
            price_     = price;
            order_qty_ = qty;            
        }

        const OrderState&
        Order::state() const
        { 
            return state_;
        }

        bool
        Order::is_buy() const
        { 
            return is_buy_;
        }

        bool
        Order::is_limit() const
        {
            return (price_ == 0);
        }

        market::book::Price
        Order::price() const
        { 
            return price_;
        }

        market::book::Quantity
        Order::order_qty() const
        {
            return order_qty_;
        }

        market::book::Quantity
        Order::open_qty() const
        {
            // If not completely filled, calculate
            if (filled_qty_ < order_qty_) {
                return order_qty_ - filled_qty_;
            // Else prevent accidental overflow
            } else {
                return 0;
            }
        }

        const market::book::Quantity&
        Order::filled_qty() const
        {
            return filled_qty_;
        }

        const market::book::Cost&
        Order::filled_cost() const
        {
            return filled_cost_;
        }

        void
        Order::fill(
            market::book::Quantity fill_qty,
            market::book::Cost fill_cost,
            market::book::FillId /*fill_id*/
        )
        {
            filled_qty_ += fill_qty;
            filled_cost_ += fill_cost;
            if (!open_qty()) {
                state_ = os_complete;
            }
        }

        void
        Order::accept()
        { 
            if (os_new == state_) {
                state_ = os_accepted;
            }
        }

        void
        Order::cancel()
        {
            if (os_complete != state_) {
                state_ = os_cancelled;
            }
        }

        void
        Order::replace(
            market::book::Quantity size_delta,
            market::book::Price new_price
        )
        {
            if (os_accepted == state_) {
                order_qty_ += size_delta;
                price_ = new_price;
            }
        }

    } // namespace model
} // namespace market
