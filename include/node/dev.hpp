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
#include "events.hpp"

namespace market { 
    namespace node { 
        namespace dev {
            class Buffer {};
            class Exception {};

            template<typename ...E>
            class Stream : public EventEmitter<E...>
            {};

            namespace net {
                class Socket : public Stream<>{};

                typedef std::tuple<
                    ev::listening, std::function<void()>,
                    ev::connection, std::function<void(Socket&)>,
                    ev::error, std::function<void(Exception&)>,
                    ev::close, std::function<void()>
                > server_events;

                template<typename ...E>
                class Server : public EventEmitter<server_events, E...>
                {
                    public:
                        typedef std::function<void()> ListeningListener;
                };
            }

            namespace http {
                typedef std::tuple<
                    ev::data, std::function<void(const std::string&)>,
                    ev::end, std::function<void()>,
                    ev::close, std::function<void()>
                > server_req_events;

                class ServerRequest : public EventEmitter<server_req_events> {
                    public:
                        typedef std::function<void(const std::string&)> DataListener;
                        typedef std::function<void()> EndListener;
                        typedef std::function<void()> CloseListener;
                };

                class ServerResponse {};

                typedef std::tuple<
                    ev::request,       std::function<void(ServerRequest&, ServerResponse&)>,
                    ev::connection,    std::function<void(net::Socket&)>,
                    ev::close,         std::function<void()>,
                    ev::checkContinue, std::function<void(ServerRequest&, ServerResponse&)>,
                    ev::upgrade,       std::function<void(ServerRequest&, net::Socket&, Buffer&)>,
                    ev::clientError,   std::function<void(Exception&)>
                > server_events;

                class Server : public net::Server<server_events> {
                    protected:
                        Server(){}

                    public:
                        virtual ~Server() {}

                    public:
                        static std::shared_ptr<Server>
                        createServer(
                            callback_type<ev::request>::type requestListener
                        )
                        {
                            return nullptr;
                        }

                        bool
                        listen(
                            int port,
                            const std::string& hostname,
                            callback_type<ev::listening>::type callback
                        )
                        {
                            return false;
                        }

                        bool
                        listen(
                            int port,
                            callback_type<ev::listening>::type callback
                        )
                        {
                            return false;
                        }

                        bool
                        close() 
                        {
                            return false;
                        }
                    private:
                };
            } // namespace http
        } // namespace dev
    } // namespace node
} // namespace market 
