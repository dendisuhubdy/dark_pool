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
#include "error.hpp"

namespace market { 
    namespace node {
        ///!
        ///  Class that represents the loop instance.
        ///
        class loop {
            public:
                ///!
                ///  Default constructor
                ///  @param use_default indicates whether to use default loop or create a new loop.
                ///
                loop(
                    bool use_default=false
                ) : uv_loop_(use_default ? uv_default_loop() : uv_loop_new())
                {}

                ///!
                ///  Destructor
                ///
                ~loop() {
                    if(uv_loop_) {
                        uv_loop_delete(uv_loop_);
                        uv_loop_ = nullptr;
                    }
                }

                ///!
                ///  Returns internal handle for libuv functions.
                ///
                uv_loop_t*
                get()
                {
                    return uv_loop_;
                }

                ///!
                ///  Starts the loop. Always returns zero.
                ///  Internally, this function just calls uv_run() function.
                ///
                bool
                run()
                {
                    return uv_run(uv_loop_)==0;
                }

                ///!
                ///  Polls for new events once. Blocking if there is no event pending.
                ///  Returns true if there is no event pending, or false if there are
                ///  more events to be polled.
                ///  Internally, this function just calls uv_run_once() function.
                ///
                bool
                run_once()
                {
                    return uv_run_once(uv_loop_)==0;
                }

                ///!
                ///  ...
                ///  Internally, this function just calls uv_update_time() function.
                ///
                void
                update_time()
                {
                    uv_update_time(uv_loop_);
                }

                ///!
                ///  ...
                ///  Internally, this function just calls uv_now() function.
                ///
                int64_t
                now() {
                    return uv_now(uv_loop_);
                }

                ///!
                ///  Returns the last error which occured in the loop.
                ///
                error
                last_error() {
                    return uv_last_error(uv_loop_);
                }

            private:
                loop(const loop&);
                void operator =(const loop&);

            private:
                uv_loop_t* uv_loop_;
        };

        ///!
        ///  Starts the default loop. Always returns true.
        ///
        inline bool
        run()
        {
            return uv_run(uv_default_loop()) == 0;
        }

        ///!
        ///  Polls for new events once for the default loop. Blocking if there is no event pending.
        ///  Returns true if there is no event pending, or false if there are
        ///  more events to be polled.
        ///
        inline bool
        run_once()
        {
            return uv_run_once(uv_default_loop()) == 0;
        }

    } // namespace node
} // namespace market
