// Copyright (c) 2012, 2013 Object Computing, Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
//
// * Neither the name of Object Computing, Inc. nor the names of its contributors
//   may be used to endorse or promote products derived from this software without 
//   specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
// OF THE POSSIBILITY OF SUCH DAMAGE

#pragma once

#include "book/order.hpp"

namespace market { 
    namespace book {

        /// @brief interface an order must implement in order to be used by OrderBook.
        /// Note: structly speaking, inheriting from Order should not be required, 
        /// due to the template implementation of OrderBook.

        template <class OrderPtr = Order*>
        class OrderListener {
            public:
                /// @brief callback for an order accept
                virtual void on_accept(const OrderPtr& order) = 0;

                /// @brief callback for an order reject
                virtual void on_reject(const OrderPtr& order, const char* reason) = 0;

                /// @brief callback for an order fill
                /// @param order the inbound order
                /// @param matched_order the matched order
                /// @param fill_qty the quantity of this fill
                /// @param fill_cost the cost of this fill (qty * price)
                virtual void on_fill(
                    const OrderPtr& order,
                    const OrderPtr& matched_order,
                    Quantity fill_qty,
                    Cost fill_cost
                ) = 0;

                /// @brief callback for an order cancellation
                virtual void on_cancel(const OrderPtr& order) = 0;

                /// @brief callback for an order cancel rejection
                virtual void on_cancel_reject(const OrderPtr& order, const char* reason) = 0;

                /// @brief callback for an order replace
                /// @param order the replaced order
                /// @param size_delta the change to order quantity
                /// @param new_price the updated order price
                virtual void on_replace(
                    const OrderPtr& order,
                    const int32_t& size_delta,
                    Price new_price
                ) = 0;

                /// @brief callback for an order replace rejection
                virtual void on_replace_reject(const OrderPtr& order, const char* reason) = 0;
        };
    } 
}
