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

#include "book/types.hpp"

namespace market { 
    namespace book {

        /// @brief a single level of the limit order book aggregated by price
        class DepthLevel {
            public:
                /// @brief construct
                DepthLevel();

                /// @brief assign
                DepthLevel& operator=(const DepthLevel& rhs);

                /// @brief get price
                const Price& price() const;
  
                /// @brief get count
                uint32_t order_count() const;
                
                /// @brief get aggregate quantity
                Quantity aggregate_qty() const;
  
                /// @brief is this level part of the excess
                bool is_excess() const { return is_excess_; }

                void init(Price price, bool is_excess);

                /// @brief add an order to the level
                /// @param qty open quantity of the order
                void add_order(Quantity qty);

                /// @brief increase the quantity of existing orders
                /// @param qty amount to increase the quantity by
                void increase_qty(Quantity qty);

                /// @brief decrease the quantity of existing orders
                /// @param qty amount to decrease the quantity by
                void decrease_qty(Quantity qty);

                /// @brief cancel or fill an order, decrease count and quantity
                /// @param qty the closed quantity
                /// @return true if the level is now empty
                bool close_order(Quantity qty);

                /// @brief set last changed stamp on this level
                void last_change(ChangeId last_change) { last_change_ = last_change; }

                /// @brief get last change stamp for this level
                ChangeId last_change() const { return last_change_; }

                /// @brief has the level changed since the given stamp?
                /// @param last_published_change the stamp to compare to
                bool changed_since(ChangeId last_published_change) const;

            private:
                Price price_;
                uint32_t order_count_;
                Quantity aggregate_qty_;
                bool is_excess_;
            public:
                ChangeId last_change_;
        };

        inline bool
        DepthLevel::changed_since(ChangeId last_published_change) const
        {
            return last_change_ > last_published_change;
        }
    } 
}
