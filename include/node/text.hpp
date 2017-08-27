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

#include <functional>
#include "base.hpp"

namespace market { 
    namespace node {
	    namespace text {
		    struct ci_less : std::binary_function<std::string, std::string, bool> {
			    // case-independent (ci) compare_less binary function
			    struct nocase_compare : public std::binary_function<unsigned char, unsigned char, bool> {
				    bool operator()(
                        const unsigned char& c1,
                        const unsigned char& c2) const
				    {
					    return tolower(c1) < tolower(c2);
				    }
			    };

			    bool operator()(
                    const std::string & s1,
                    const std::string & s2
                ) const
			    {
				    return std::lexicographical_compare(
                        s1.begin(),
                        s1.end(), // source range
                        s2.begin(),
                        s2.end(), // dest range
                        nocase_compare()
                    ); // comparison
			    }
		    };
	    } // namespace text
    } // namespace node
} // namespace market
