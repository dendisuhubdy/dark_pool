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

#include"base.hpp"

namespace market { 
    namespace node {
        class exception {
            public:
                exception(
                    const std::string& message
                ) : message_(message)
                {}

                virtual ~exception() {}

                const
                std::string& message() const
                { 
                    return message_;
                }

            private:
                std::string message_;
        };

        class error {
            public:
                error() : uv_err_() {}
                error(uv_err_t e) : uv_err_(e) {}
                error(uv_err_code c) : uv_err_{ c, 0 } {}
                error(int c) : uv_err_{ static_cast<uv_err_code>(c), 0 } { }
                ~error() = default;

            public:
                operator
                bool()
                {
                    return uv_err_.code != UV_OK;
                }

                uv_err_code code() const
                {
                    return uv_err_.code;
                }

                const char*
                name() const {
                    return uv_err_name(uv_err_);
                }

                const char*
                str() const
                {
                    return uv_strerror(uv_err_);
                }

            private:
                uv_err_t uv_err_;
        };

	    inline error
        get_last_error()
        {
            return uv_last_error(uv_default_loop());
        }
    } // namespace node
} // namespace market
