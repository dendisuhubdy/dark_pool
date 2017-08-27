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
#include "handle.hpp"
#include "callback.hpp"

//FIXME: remove this.
#pragma clang diagnostic ignored "-Wtautological-compare"

namespace market { 
    namespace node {
	    namespace base {
		    class stream : public handle {
		        public:
			        template<typename T>
			        stream(
                        T* x
                    ) : handle(x)
                    {}

                    bool
                    listen(
                        std::function<void(market::node::error)> callback,
                        int backlog=128
                    )
			        {
                        callbacks::store(
                            get()->data,
                            market::node::internal::uv_cid_listen,
                            callback
                        );

                        return uv_listen(
                            get<uv_stream_t>(),
                            backlog,
                            [](uv_stream_t* s, int status) {
                                callbacks::invoke<decltype(callback)>(
                                    s->data,
                                    market::node::internal::uv_cid_listen,
                                    status ? uv_last_error(s->loop) : error()
                                );
                            }
                        ) == 0;
                    }

                    bool
                    accept(
                        stream* client
                    )
                    {
                        return uv_accept(
                            get<uv_stream_t>(),
                            client->get<uv_stream_t>()
                        ) == 0;
                    }

                    bool read_start(
                        std::function<void(const char* buf, ssize_t len)> callback
                    )
                    {
                        return read_start<0>(callback);
                    }

                    template<int max_alloc_size>
                    bool
                    read_start(
                        std::function<void(const char* buf, ssize_t len)> callback
                    )
                    {
                        callbacks::store(
                            get()->data,
                            market::node::internal::uv_cid_read_start,
                            callback
                        );

                        return uv_read_start(
                            get<uv_stream_t>(),
                            [](uv_handle_t*, size_t suggested_size) {
                                if (!max_alloc_size) {
                                    return uv_buf_t { 
                                        new char[suggested_size], suggested_size
                                    };
                                } else {
                                    auto size = max_alloc_size > suggested_size ? suggested_size : max_alloc_size;
                                    return uv_buf_t { new char[size], size };
                                }
                            },
                            [](uv_stream_t* s, ssize_t nread, uv_buf_t buf) {
                                if(nread < 0) {
                                    assert(uv_last_error(s->loop).code == UV_EOF);
                                    callbacks::invoke<decltype(callback)>(
                                        s->data,
                                        market::node::internal::uv_cid_read_start,
                                        nullptr,
                                        nread
                                    );
                                } else if(nread >= 0) {
                                    callbacks::invoke<decltype(callback)>(
                                        s->data,
                                        market::node::internal::uv_cid_read_start,
                                        buf.base,
                                        nread
                                    );
                                }
                                delete buf.base;
                            }
                        ) == 0;
                    }

                    bool
                    read_stop()
                    {
                        return uv_read_stop(get<uv_stream_t>()) == 0;
                    }
                        
                    // TODO: implement read2_start()

                    bool
                    write(
                        const char* buf,
                        size_t len,
                        std::function<void(error)> callback
                    )
                    {
                        uv_buf_t bufs[] = {
                            uv_buf_t { const_cast<char*>(buf), len }
                        };

                        callbacks::store(
                            get()->data,
                            market::node::internal::uv_cid_write,
                            callback
                        );

                        return uv_write(
                            new uv_write_t,
                            get<uv_stream_t>(),
                            bufs,
                            1,
                            [](uv_write_t* req, int status) {
                                callbacks::invoke<decltype(callback)>(
                                    req->handle->data,
                                    market::node::internal::uv_cid_write,
                                    status ? uv_last_error(req->handle->loop) : error()
                                );
                                delete req;
                            }
                        ) == 0;
                    }

                    bool
                    write(
                        const std::string& buf,
                        std::function<void(error)> callback
                    )
                    {
                        uv_buf_t bufs[] = {
                            uv_buf_t {
                                const_cast<char*>(buf.c_str()), buf.length()
                            }
                        };

                        callbacks::store(
                            get()->data,
                            market::node::internal::uv_cid_write,
                            callback
                        );

                        return uv_write(
                            new uv_write_t,
                            get<uv_stream_t>(),
                            bufs,
                            1,
                            [](uv_write_t* req, int status) {
                                callbacks::invoke<decltype(callback)>(
                                    req->handle->data,
                                    market::node::internal::uv_cid_write,
                                    status ? uv_last_error(req->handle->loop) : error()
                                );
                                delete req;
                            }
                        ) == 0;
                    }

                    bool
                    write(
                        const std::vector<char>& buf,
                        std::function<void(error)> callback
                    )
                    {
                        uv_buf_t bufs[] = {
                            uv_buf_t {
                                const_cast<char*>(&buf[0]), buf.size()
                            }
                        };

                        callbacks::store(
                            get()->data,
                            market::node::internal::uv_cid_write,
                            callback
                        );

                        return uv_write(
                            new uv_write_t,
                            get<uv_stream_t>(),
                            bufs,
                            1,
                            [](uv_write_t* req, int status) {
                                callbacks::invoke<decltype(callback)>(
                                    req->handle->data,
                                    market::node::internal::uv_cid_write,
                                    status ? uv_last_error(req->handle->loop) : error()
                                );
                                delete req;
                            }
                        ) == 0;
                    }

                    // TODO: implement write2()

                    bool shutdown(
                        std::function<void(error)> callback
                    )
                    {
                        callbacks::store(
                            get()->data,
                            market::node::internal::uv_cid_shutdown,
                            callback
                        );

                        return uv_shutdown(
                            new uv_shutdown_t,
                            get<uv_stream_t>(),
                            [](uv_shutdown_t* req, int status) {
                                callbacks::invoke<decltype(callback)>(
                                    req->handle->data,
                                    market::node::internal::uv_cid_shutdown,
                                    status ? uv_last_error(req->handle->loop) : error()
                                );
                                delete req;
                            }
                        ) == 0;
                    }
	        };
        } // namespace base
    } // namespace node
} // namespace market
