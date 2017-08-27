#pragma once

#include <iostream>

#include "model/order.hpp"
#include "book/depth_order_book.hpp"
#include "disruptor/interface.hpp"
#include "disruptor/ring_buffer.hpp"

namespace market { 
    namespace model { 

        // @brief binding of DepthOrderBook template with Order pointer.
        class OrderBook : public market::book::DepthOrderBook<market::model::Order*, 5>
        {
            public:
                OrderBook(
                    market::disruptor::RingBuffer<market::model::Order>* ring_buffer,
                    market::disruptor::SequenceBarrierInterface* sequence_barrier,
                    market::disruptor::EventHandlerInterface<market::model::Order>* event_handler,
                    market::disruptor::ExceptionHandlerInterface<market::model::Order>* exception_handler
                ) : running_(false),
                    ring_buffer_(ring_buffer),
                    sequence_barrier_(sequence_barrier),
                    event_handler_(event_handler),
                    exception_handler_(exception_handler),
                    fill_id_(0)
                {}
    
                typedef market::book::Callback<market::model::Order*> MarketCallback;
                typedef uint32_t FillId;

                virtual market::disruptor::Sequence*
                GetSequence()
                {   
                    return &sequence_;
                }

                virtual void
                Halt()
                {   
                    running_.store(false);
                    sequence_barrier_->Alert();
                }

                virtual void
                Run()
                {   
                    if (running_.load()) {
                        throw std::runtime_error("Thread is already running");
                    }
                    running_.store(true);
                    sequence_barrier_->ClearAlert();
                    event_handler_->OnStart();

                    market::model::Order* order = NULL;
                    int64_t next_sequence = sequence_.sequence() + 1L;

                    while (true) {
                        try {
                            int64_t available_sequence = sequence_barrier_->WaitFor(next_sequence);

                            while (next_sequence <= available_sequence) {
                                order = ring_buffer_->Get(next_sequence);
                                event_handler_->OnEvent(
                                    next_sequence,
                                    next_sequence == available_sequence,
                                    order
                                );
                                next_sequence++;
                            }

                            sequence_.set_sequence(next_sequence - 1L);
                        } catch(const market::disruptor::AlertException& e) {
                            if (!running_.load()) {
                                break;
                            }
                        } catch(const std::exception& e) {
                            exception_handler_->Handle(e, next_sequence, order);
                            sequence_.set_sequence(next_sequence);
                            next_sequence++;
                        }
                    }
                    event_handler_->OnShutdown();
                    running_.store(false);
                }

                void operator()() { Run(); }

                // Override callback handling to update Order state
                virtual void perform_callback(MarketCallback& cb);
            private:
                FillId fill_id_;

                std::atomic<bool> running_;
                market::disruptor::Sequence sequence_;

                market::disruptor::RingBuffer<market::model::Order>* ring_buffer_;
                market::disruptor::SequenceBarrierInterface* sequence_barrier_;
                market::disruptor::EventHandlerInterface<market::model::Order>* event_handler_;
                market::disruptor::ExceptionHandlerInterface<market::model::Order>* exception_handler_;

                DISALLOW_COPY_AND_ASSIGN(OrderBook);
        };

        inline void
        OrderBook::perform_callback(MarketCallback& cb)
        {
            book::DepthOrderBook<Order*, 5>::perform_callback(cb);
            switch(cb.type) {
                case MarketCallback::cb_order_accept: {
                    cb.order->accept();
                    break;
                }
                case MarketCallback::cb_order_fill: {
                    // Increment fill ID once
                    ++fill_id_;
                    // Update the orders
                    market::book::Cost fill_cost = cb.fill_qty * cb.fill_price;
                    cb.matched_order->fill(cb.fill_qty, fill_cost, fill_id_);
                    cb.order->fill(cb.fill_qty, fill_cost, fill_id_);
                    break;
                }
                case MarketCallback::cb_order_cancel: {
                    cb.order->cancel();
                    break;
                }
                case MarketCallback::cb_order_replace: {
                    // Modify the order itself
                    cb.order->replace(cb.repl_size_delta, cb.repl_new_price);
                    break;
                }
                default: break;
            }
        }
    
   } // namespace model
} // namespace market
