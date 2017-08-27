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

#include "book/depth_level.hpp"
#include "book/types.hpp"
#include <stdexcept>

namespace market { 
    namespace book {

        DepthLevel::DepthLevel() : price_(INVALID_LEVEL_PRICE),
                                   order_count_(0),
                                   aggregate_qty_(0)
        {}

        DepthLevel&
        DepthLevel::operator=(const DepthLevel& rhs)
        {
            price_ = rhs.price_;
            order_count_ = rhs.order_count_;
            aggregate_qty_ = rhs.aggregate_qty_;
            if (rhs.price_ != INVALID_LEVEL_PRICE) {
                last_change_ = rhs.last_change_;
            }
            // Do not copy is_excess_
            return *this;
        }

        const Price&
        DepthLevel::price() const
        {
            return price_;
        }

        void
        DepthLevel::init(
            Price price,
            bool is_excess
        )
        {
            price_ = price;
            order_count_ = 0;
            aggregate_qty_ = 0;
            is_excess_ = is_excess;
        }

        uint32_t
        DepthLevel::order_count() const
        {
            return order_count_;
        }

        Quantity
        DepthLevel::aggregate_qty() const
        {
            return aggregate_qty_;
        }

        void
        DepthLevel::add_order(Quantity qty)
        {
            // Increment/increase
            ++order_count_;
            aggregate_qty_ += qty;
        }

        bool
        DepthLevel::close_order(Quantity qty)
        {
            bool empty = false;
            // If this is the last order, reset the level
            if (order_count_ == 0) {
                throw std::runtime_error("DepthLevel::close_order order count too low");
            } else if (order_count_ == 1) {
                order_count_ = 0;
                aggregate_qty_ = 0;
                empty = true;
            // Else, decrement/decrease
            } else {
                --order_count_;
                if (aggregate_qty_ >= qty) {
                    aggregate_qty_ -= qty;
                } else {
                    throw std::runtime_error("DepthLevel::close_order level quantity too low");
                }
            }
            return empty;
        }

        void
        DepthLevel::increase_qty(Quantity qty)
        {
            aggregate_qty_ += qty;
        }

        void
        DepthLevel::decrease_qty(Quantity qty)
        {
            aggregate_qty_ -= qty;
        }
    } 
}

