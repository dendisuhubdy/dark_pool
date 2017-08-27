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
#include "handle.hpp"
#include "stream.hpp"
#include "net.hpp"
#include "callback.hpp"

namespace market { 
    namespace node {
        namespace net {
            class tcp : public market::node::base::stream {
                public:
                    template<typename X>
                    tcp(
                        X* x
                    ) : stream(x)
                    {}

                public:
                    tcp() : market::node::base::stream(new uv_tcp_t)
                    {
                        uv_tcp_init(uv_default_loop(), get<uv_tcp_t>());
                    }

                    tcp(
                        market::node::loop& l
                    ) : market::node::base::stream(new uv_tcp_t)
                    {
                        uv_tcp_init(l.get(), get<uv_tcp_t>());
                    }

                    static std::shared_ptr<tcp> create()
                    {
                        return std::shared_ptr<tcp>(new tcp);
                    }

                // TODO: bind and listen
                    static std::shared_ptr<tcp>
                    create_server(const std::string& ip, int port) {
                    return nullptr;
                }

                    bool
                    nodelay(
                        bool enable
                    )
                    {
                        return uv_tcp_nodelay(
                            get<uv_tcp_t>(),
                            enable ? 1 : 0 
                        ) == 0;
                    }

                    bool
                    keepalive(
                        bool enable,
                        unsigned int delay
                    )
                    {
                        return uv_tcp_keepalive(
                            get<uv_tcp_t>(),
                            enable ? 1 : 0,
                            delay
                        ) == 0;
                    }

                    bool
                    simultaneous_accepts(
                        bool enable
                    )
                    {
                        return uv_tcp_simultaneous_accepts(
                            get<uv_tcp_t>(),
                            enable ? 1 : 0
                        ) == 0;
                    }

                    bool
                    bind(
                        const std::string& ip,
                        int port
                    )
                    {
                        return uv_tcp_bind(
                            get<uv_tcp_t>(),
                            uv_ip4_addr(ip.c_str(), port)
                        ) == 0;
                    }

                    bool
                    bind6(
                        const std::string& ip,
                        int port
                    )
                    {
                        return uv_tcp_bind6(
                            get<uv_tcp_t>(),
                            uv_ip6_addr(ip.c_str(), port)
                        ) == 0;
                    }

                    bool
                    connect(
                        const std::string& ip,
                        int port, std::function<void(error)> callback
                    )
                    {
                        callbacks::store(
                            get()->data,
                            market::node::internal::uv_cid_connect,
                            callback
                        );
                        return uv_tcp_connect(
                            new uv_connect_t,
                            get<uv_tcp_t>(),
                            to_ip4_addr(ip, port),
                            [](uv_connect_t* req, int status) {
                                callbacks::invoke<decltype(callback)>(
                                    req->handle->data,
                                    market::node::internal::uv_cid_connect,
                                    status ? uv_last_error(req->handle->loop) : error()
                                );
                                delete req;
                            }
                        ) == 0;
                    }

                    bool
                    connect6(
                        const std::string& ip,
                        int port, std::function<void(error)> callback
                    )
                    {
                        callbacks::store(
                            get()->data,
                            market::node::internal::uv_cid_connect6,
                            callback
                        );

                        return uv_tcp_connect6(
                            new uv_connect_t,
                            get<uv_tcp_t>(),
                            to_ip6_addr(ip, port),
                            [](uv_connect_t* req, int status) {
                                callbacks::invoke<decltype(callback)>(
                                    req->handle->data,
                                    market::node::internal::uv_cid_connect6,
                                    status ? uv_last_error(req->handle->loop) : error()
                                );
                                delete req;
                            }
                        ) == 0;
                    }

                    bool
                    getsockname(
                        bool& ip4,
                        std::string& ip,
                        int& port
                    )
                    {
                        struct sockaddr_storage addr;
                        int len = sizeof(addr);
                        if (uv_tcp_getsockname(get<uv_tcp_t>(), reinterpret_cast<struct sockaddr*>(&addr), &len) == 0)
                        {
                            ip4 = (addr.ss_family == AF_INET);
                            if(ip4) { 
                                return from_ip4_addr(reinterpret_cast<ip4_addr*>(&addr), ip, port);
                            } else { 
                                return from_ip6_addr(reinterpret_cast<ip6_addr*>(&addr), ip, port);
                            }
                        }
                        return false;
                    }

                    bool
                    getpeername(
                        bool& ip4,
                        std::string& ip,
                        int& port
                    )
                    {
                        struct sockaddr_storage addr;
                        int len = sizeof(addr);
                        if(uv_tcp_getpeername(get<uv_tcp_t>(), reinterpret_cast<struct sockaddr*>(&addr), &len) == 0)
                        {
                            ip4 = (addr.ss_family == AF_INET);
                            if(ip4) { 
                                return from_ip4_addr(reinterpret_cast<ip4_addr*>(&addr), ip, port);
                            } else { 
                                return from_ip6_addr(reinterpret_cast<ip6_addr*>(&addr), ip, port);
                            }
                            return false;
                        }
                    }
            }; 
        } // namespace net
    } // namespace node 
} // namespace market
