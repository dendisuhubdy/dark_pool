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
#include "callback.hpp"

namespace market { 
    namespace node {
	    namespace net {
		    typedef sockaddr_in ip4_addr;
		    typedef sockaddr_in6 ip6_addr;

		    inline ip4_addr
            to_ip4_addr(
                const std::string& ip,
                int port
            )
            {
                return uv_ip4_addr(ip.c_str(), port);
            }

		    inline ip6_addr
            to_ip6_addr(
                const std::string& ip,
                int port
            )
            {
                return uv_ip6_addr(ip.c_str(), port);
            }

            inline bool
            from_ip4_addr(
                ip4_addr* src,
                std::string& ip,
                int& port
            )
            {
			    char dest[16];
			    if(uv_ip4_name(src, dest, 16) == 0) {
				    ip = dest;
				    port = static_cast<int>(ntohs(src->sin_port));
				    return true;
			    }
			    return false;
		    }

		    inline bool
            from_ip6_addr(
                ip6_addr* src,
                std::string& ip,
                int& port
            )
            {
                char dest[46];
                if(uv_ip6_name(src, dest, 46) == 0) {
                    ip = dest;
                    port = static_cast<int>(ntohs(src->sin6_port));
                    return true;
                }
                return false;
            }

	    } // namespace net
    } // namespace node
} // namespace market
