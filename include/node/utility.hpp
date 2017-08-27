// Copyright (c) 2012 Daniel Kang. All rights reserved.
// Copyright (c) 2013 Alex Caudill. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#pragma once

#include "base.hpp"
#include <tuple>

namespace market { 
    namespace node { 
        namespace dev {
            namespace util {
                // Merges two std::tuple<>s.
                template<typename...>
                struct tuple_merge;

                template<typename ...TA, typename ...TB>
                struct tuple_merge<std::tuple<TA...>, std::tuple<TB...>>
                {
                    typedef std::tuple<TA..., TB...> type;
                };

                template<typename ...TA>
                struct tuple_merge<std::tuple<TA...>>
                {
                    typedef std::tuple<TA...> type;
                };

                template<>
                struct tuple_merge<>
                {
                    typedef std::tuple<> type;
                };

                // Appends an element to std::tuple<>.
                template<typename, typename>
                struct tuple_append;

                template<typename A, typename ...T>
                struct tuple_append<A, std::tuple<T...> >
                {
                    typedef std::tuple<A, T...> type;
                };

                // Prepends an element to std::tuple<>.
                template<typename, typename >
                struct tuple_prepend;

                template<typename ...T, typename A>
                struct tuple_prepend<std::tuple<T...>, A>
                {
                    typedef std::tuple<T..., A> type;
                };

                // make a new tuple from elements at even index
                template<typename>
                struct tuple_even_elements;

                template<typename A, typename B>
                struct tuple_even_elements<std::tuple<A, B>>
                {
                    typedef std::tuple<A> type;
                };

                template<typename A, typename B, typename ...T>
                struct tuple_even_elements<std::tuple<A, B, T...>>
                {
                    typedef typename tuple_append<A,
                    typename tuple_even_elements<std::tuple<T...>>::type>::type type;
                };

                template<>
                struct tuple_even_elements<std::tuple<>>
                {
                    typedef std::tuple<> type;
                };

                // make a new tuple from elements at odd index
                template<typename>
                struct tuple_odd_elements;

                template<typename A, typename B>
                struct tuple_odd_elements<std::tuple<A, B>>
                {
                    typedef std::tuple<B> type;
                };

                template<typename A, typename B, typename ...T>
                struct tuple_odd_elements<std::tuple<A, B, T...>>
                {
                    typedef typename tuple_append<B,
                    typename tuple_odd_elements<std::tuple<T...>>::type>::type type;
                };

                template<>
                struct tuple_odd_elements<std::tuple<>>
                {
                    typedef std::tuple<> type;
                };

                // get the first index of a type in the tuple
                template<typename T, typename C, std::size_t I>
                struct tuple_index_r;

                template<typename H, typename ...R, typename C, std::size_t I>
                struct tuple_index_r<std::tuple<H, R...>, C, I> :
                    public std::conditional<std::is_same<C, H>::value,
                    std::integral_constant<std::size_t, I>,
                    tuple_index_r<std::tuple<R...>, C, I+1>>::type
                {};

                template<typename C, std::size_t I>
                struct tuple_index_r<std::tuple<>, C, I>
                {};

                template<typename T, typename C>
                struct tuple_index_of : 
                    public std::integral_constant<std::size_t, tuple_index_r<T, C, 0>::value> {};
            } // namespace util
        } // namespace dev
    } // namespace node
} // namespace market
