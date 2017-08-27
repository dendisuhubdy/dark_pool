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

#include <stdint.h>

namespace market { 
    namespace book {
        // Types used in Liquibook
        typedef uint32_t Price;
        typedef uint32_t Quantity;
        typedef uint32_t Cost;
        typedef uint32_t FillId;
        typedef uint32_t ChangeId;
        typedef uint32_t TransId;
        typedef uint32_t OrderConditions;

        enum OrderCondition {
            oc_all_or_none = 1,
            oc_immediate_or_cancel = oc_all_or_none * 2
        };

        // Constants used in liquibook
        extern const Price INVALID_LEVEL_PRICE;
        extern const Price MARKET_ORDER_PRICE;
        extern const Price MARKET_ORDER_BID_SORT_PRICE;
        extern const Price MARKET_ORDER_ASK_SORT_PRICE;
        extern const Price PRICE_UNCHANGED;

        extern const int32_t SIZE_UNCHANGED;
    } 
} // namespace
