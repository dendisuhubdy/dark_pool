// Copyright John W. Wilkinson 2007 - 2011
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "json/writer.hpp"
#include "json_spirit/writer_template.hpp"

using namespace json_spirit;

#ifdef JSON_SPIRIT_VALUE_ENABLED
    void json_spirit::write( const Value& value, std::ostream& os, unsigned int options )
    {
        write_stream( value, os, options );
    }

    std::string json_spirit::write( const Value& value, unsigned int options )
    {
        return write_string( value, options );
    }

    void json_spirit::write_formatted( const Value& value, std::ostream& os )
    {
        write_stream( value, os, pretty_print );
    }

    std::string json_spirit::write_formatted( const Value& value )
    {
        return write_string( value, pretty_print );
    }
#endif

#if defined( JSON_SPIRIT_WVALUE_ENABLED ) && !defined( BOOST_NO_STD_WSTRING )
    void json_spirit::write_formatted( const wValue& value, std::wostream& os )
    {
        write_stream( value, os, pretty_print );
    }

    std::wstring json_spirit::write_formatted( const wValue& value )
    {
        return write_string( value, pretty_print );
    }

    void json_spirit::write( const wValue& value, std::wostream& os, unsigned int options )
    {
        write_stream( value, os, options );
    }

    std::wstring json_spirit::write( const wValue& value, unsigned int options )
    {
        return write_string( value, options );
    }
#endif
