// Copyright (c) 2001-2010 quickfixengine.org  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the
//    distribution.
//
// 3. The end-user documentation included with the redistribution,
//    if any, must include the following acknowledgment:
//      "This product includes software developed by
//       quickfixengine.org (http://www.quickfixengine.org/)."
//   Alternately, this acknowledgment may appear in the software itself,
//   if and wherever such third-party acknowledgments normally appear.
//
// 4. The names "QuickFIX" and "quickfixengine.org" must
//    not be used to endorse or promote products derived from this
//    software without prior written permission. For written
//    permission, please contact ask@quickfixengine.org
//
// 5. Products derived from this software may not be called "QuickFIX",
//    nor may "QuickFIX" appear in their name, without prior written
//    permission of quickfixengine.org
//
// THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.  IN NO EVENT SHALL QUICKFIXENGINE.ORG OR
// ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

#ifndef FIX_HTTPMESSAGE
#define FIX_HTTPMESSAGE

#ifdef _MSC_VER
#pragma warning( disable: 4786 )
#endif

#include "Exceptions.h"
#include <map>

namespace FIX
{
/**
 * HTTP Message that implemented GET functionality
 */
class HttpMessage
{
public:
  typedef std::map<std::string, std::string> Parameters;

  HttpMessage();

  /// Construct a message from a string
  HttpMessage( const std::string& string )
  throw( InvalidMessage );

  HttpMessage( const HttpMessage& copy )
  {
    m_root = copy.m_root;
    m_parameters = copy.m_parameters;
  }

public:
  /// Get a string representation of the message
  std::string toString() const;
  /// Get a string representation without making a copy
  std::string& toString( std::string& ) const;

  void setString( const std::string& string )
  throw( InvalidMessage );

  void clear()
  {
#if defined(_MSC_VER) && _MSC_VER < 1300
    m_root = "";
#else
    m_root.clear();
#endif
    m_parameters.clear();
  }

  const std::string& getRootString() const
  { return m_root; }

  const std::string getParameterString() const
  {
    std::string result;
    Parameters::const_iterator i;
    for( i = m_parameters.begin(); i != m_parameters.end(); ++i )
    {
      result += (i == m_parameters.begin()) ? "?" : "&";
      result += i->first + "=" + i->second;
    }
    return result;
  }

  const Parameters& getParameters() const
  { return m_parameters; }

  bool hasParameter( const std::string& key ) const
  {
    Parameters::const_iterator find = m_parameters.find( key );
    return find != m_parameters.end();
  }

  const std::string& getParameter( const std::string& key ) const
  throw( std::logic_error )
  {
    Parameters::const_iterator find = m_parameters.find( key );
    if( find == m_parameters.end() )
      throw std::logic_error( "Parameter " + key + " not found" );
    return find->second;
  }

  void addParameter( const std::string& key, const std::string& value )
  {
    m_parameters[key] = value;
  }

  void removeParameter( const std::string& key )
  {
    m_parameters.erase( key );
  }  

  static std::string createResponse( int error = 0, const std::string& text = "" );
 
private:
  std::string m_root;
  Parameters m_parameters;
};
/*! @} */

inline std::ostream& operator <<
( std::ostream& stream, const HttpMessage& message )
{
  std::string str;
  stream << message.toString( str );
  return stream;
}
}

#endif //FIX_HTTPMESSAGE
