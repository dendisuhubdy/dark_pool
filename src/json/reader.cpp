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

#include "json/reader.hpp"
#include "json/reader_template.hpp"

using namespace json_spirit;

#ifdef JSON_SPIRIT_VALUE_ENABLED
    bool json_spirit::read( const std::string& s, Value& value )
    {
        return read_string( s, value );
    }

    void json_spirit::read_or_throw( const std::string& s, Value& value )
    {
        read_string_or_throw( s, value );
    }

    bool json_spirit::read( std::istream& is, Value& value )
    {
        return read_stream( is, value );
    }

    void json_spirit::read_or_throw( std::istream& is, Value& value )
    {
        read_stream_or_throw( is, value );
    }

    bool json_spirit::read( std::string::const_iterator& begin, std::string::const_iterator end, Value& value )
    {
        return read_range( begin, end, value );
    }

    void json_spirit::read_or_throw( std::string::const_iterator& begin, std::string::const_iterator end, Value& value )
    {
        begin = read_range_or_throw( begin, end, value );
    }
#endif

#if defined( JSON_SPIRIT_WVALUE_ENABLED ) && !defined( BOOST_NO_STD_WSTRING )
    bool json_spirit::read( const std::wstring& s, wValue& value )
    {
        return read_string( s, value );
    }

    void json_spirit::read_or_throw( const std::wstring& s, wValue& value )
    {
        read_string_or_throw( s, value );
    }

    bool json_spirit::read( std::wistream& is, wValue& value )
    {
        return read_stream( is, value );
    }

    void json_spirit::read_or_throw( std::wistream& is, wValue& value )
    {
        read_stream_or_throw( is, value );
    }

    bool json_spirit::read( std::wstring::const_iterator& begin, std::wstring::const_iterator end, wValue& value )
    {
        return read_range( begin, end, value );
    }

    void json_spirit::read_or_throw( std::wstring::const_iterator& begin, std::wstring::const_iterator end, wValue& value )
    {
        begin = read_range_or_throw( begin, end, value );
    }
#endif
