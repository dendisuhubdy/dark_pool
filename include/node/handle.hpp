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
	    namespace base {
		    class handle;

		    void _delete_handle(uv_handle_t* h);

		    class handle {
		        public:
			        template<typename T>
			        handle(
                        T* x
                    ) : uv_handle_(reinterpret_cast<uv_handle_t*>(x))
			        {
				        //printf("handle(): %x\n", this);
				        assert(uv_handle_);

				        uv_handle_->data = new market::node::callbacks(market::node::internal::uv_cid_max);
				        assert(uv_handle_->data);
			        }

			        virtual
                    ~handle()
                    {
				        //printf("~handle(): %x\n", this);
				        uv_handle_ = nullptr;
			        }

			        handle(
                        const handle& h
                    ) : uv_handle_(h.uv_handle_)
			        {
				        //printf("handle(const handle&): %x\n", this);
			        }

		        public:
			        template<typename T=uv_handle_t>
			        T*
                    get()
                    {
                        return reinterpret_cast<T*>(uv_handle_);
                    }

			        template<typename T=uv_handle_t>
			        const T*
                    get() const
                    {
                        return reinterpret_cast<T*>(uv_handle_);
                    }

			        bool
                    is_active() {
                        return uv_is_active(get()) != 0;
                    }

			        void
                    close(
                        std::function<void()> callback
                    )
			        {
				        callbacks::store(
                            get()->data,
                            market::node::internal::uv_cid_close,
                            callback
                        );
				        uv_close(
                            get(),
					        [](uv_handle_t* h) {
						        callbacks::invoke<decltype(callback)>(h->data, market::node::internal::uv_cid_close);
						        _delete_handle(h);
					        }
                        );
			        }

			        handle& operator =(const handle& h)
			        {
				        uv_handle_ = h.uv_handle_;
				        return *this;
			        }

		        protected:
			        uv_handle_t* uv_handle_;
		    };

		    inline void 
            _delete_handle(
                uv_handle_t* h
            )
		    {
			    assert(h);

			    // clean up SCM
			    if(h->data) {
				    delete reinterpret_cast<callbacks*>(h->data);
				    h->data = nullptr;
			    }

			    switch(h->type) {
				    case UV_TCP: { 
                        delete reinterpret_cast<uv_tcp_t*>(h);
                        break;
                    }
				    default: {
                        assert(0);
                        break;
                    }
			    }
		    }
	    } // namespace base
    } // namespace node 
} // namespace market 
