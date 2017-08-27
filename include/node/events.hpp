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
#include "utility.hpp"

#include <exception>

namespace market { 
    namespace node { 
        namespace dev {
            namespace ev {
                struct exit {};
                struct uncaughtException {};
                struct newListener {};
                struct data {};
                struct end {};
                struct error {};
                struct close {};
                struct drain {};
                struct pipe {};
                struct secure {};
                struct secureConnection {};
                struct clientError {};
                struct secureConnect {};
                struct open {};
                struct change {};
                struct listening {};
                struct connection {};
                struct connect {};
                struct timeout {};
                struct message {};
                struct request {};
                struct checkContinue {};
                struct upgrade {};
                struct response {};
                struct socket {};
                struct continue_ {};
                struct line {};
                struct death {};
                struct debug1 {};
                struct debug2 {};
            }

            template<typename ...M>
            class EventEmitter {
                typedef typename util::tuple_merge<M...>::type merged_map;
                typedef typename util::tuple_even_elements<merged_map>::type events;
                typedef typename util::tuple_odd_elements<merged_map>::type callbacks;

                template<typename E>
                struct event_idx : public std::integral_constant<std::size_t, util::tuple_index_of<events, E>::value> {};

                template<typename E>
                struct callback_type { 
                    typedef typename std::tuple_element<event_idx<E>::value, callbacks>::type type; 
                };

                public:
                    typedef void* listener_t;

                    EventEmitter() : set_() {}
                    virtual ~EventEmitter() {}

                    template<typename E>
                    auto
                    addListener(
                        typename callback_type<E>::type callback
                    )-> decltype(&callback)
                    {
                        auto& entry = std::get<event_idx<E>::value>(set_);
                        auto ptr = new decltype(callback)(callback);
                        entry.push_back(std::shared_ptr<decltype(callback)>(ptr));
                        return ptr;
                    }

                    template<typename E>
                    bool removeListener(
                        typename callback_type<E>::type* callback_ptr
                    )
                    {
                        auto& entry = std::get<event_idx<E>::value>(set_);
                        auto it = entry.begin();
                        for(;it!=entry.end();++it) {
                            if(it->get() == callback_ptr) {
                                entry.erase(it);
                                return true;
                            }
                        }
                        return false;
                    }

                    template<typename E>
                    void removeAllListeners()
                    {
                        auto& entry = std::get<event_idx<E>::value>(set_);
                        entry.clear();
                    }

                    template<typename E, typename ...A>
                    void emit(
                        A&&... args
                    )
                    {
                        auto& entry = std::get<event_idx<E>::value>(set_);
                        for(auto x : entry) {
                            try {
                                (*x)(std::forward<A>(args)...);
                            } catch(...) {
                                // TODO: handle exception raised while executing the callbacks
                            }
                        }
                    }

                private:
                    template<typename>
                    struct set_t;

                    template<typename ...T>
                    struct set_t<std::tuple<T...>> {
                        typedef std::tuple<std::list<std::shared_ptr<T>>...> type;
                    };

                    typename set_t<callbacks>::type set_;
            };
        } // namespace dev
    } // namespace node
} // namespace market
