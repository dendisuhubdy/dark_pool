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

namespace market { 
    namespace node {
        namespace internal {
            class callback_object_base {
                public:
                    callback_object_base(
                        void* data
                    ) : data_(data)
                    {}

                    virtual
                    ~callback_object_base()
                    {}

                    void *
                    get_data()
                    { 
                        return data_;
                    }

                private:
                    void* data_;
            };

            // SCO: serialized callback object
            template<typename callback_t>
            class callback_object : public callback_object_base {
                public:
                    callback_object(
                        const callback_t& callback,
                        void* data=nullptr
                    ) : callback_object_base(data),
                        callback_(callback)
                    {}

                    virtual ~callback_object() {}

                public:
                    template<typename ...A>
                    typename std::result_of<callback_t(A...)>::type
                    invoke(
                        A&& ... args
                    )
                    {
                        return callback_(std::forward<A>(args)...);
                    }

                private:
                    callback_t callback_;
            };
        } // namespace internal

        typedef std::shared_ptr<internal::callback_object_base> callback_object_ptr;

        class callbacks {
            public:
                callbacks(
                    int max_callbacks
                ) : lut_(max_callbacks)
                {}

                 ~callbacks(){}

                template<typename callback_t>
                static void
                store(
                    void* target,
                    int cid,
                    const callback_t& callback,
                    void* data=nullptr
                )
                {
                    reinterpret_cast<callbacks*>(target)->lut_[cid] = callback_object_ptr(
                        new internal::callback_object<callback_t>(callback, data)
                    );
                }

                template<typename callback_t>
                static void* 
                get_data(
                    void* target,
                    int cid
                )
                {
                    return reinterpret_cast<callbacks*>(target)->lut_[cid]->get_data();
                }

                template<typename callback_t, typename ...A>
                static typename std::result_of<callback_t(A...)>::type
                invoke(
                    void* target,
                    int cid,
                    A&& ... args
                )
                {
                    auto x = dynamic_cast<internal::callback_object<callback_t>*>(
                        reinterpret_cast<callbacks*>(target)->lut_[cid].get()
                    );
                    assert(x);
                    return x->invoke(std::forward<A>(args)...);
                }

            private:
                std::vector<callback_object_ptr> lut_;
        };
    } // namespace node
} //namespace market
