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

#ifndef FIX_SESSIONID_H
#define FIX_SESSIONID_H

#include "Fields.h"

namespace FIX
{
/// Unique session id consists of BeginString, SenderCompID and TargetCompID.
class SessionID
{
public:
  SessionID()
  {
        toString(m_frozenString);
  }

  SessionID( const std::string& beginString,
             const std::string& senderCompID,
             const std::string& targetCompID,
             const std::string& sessionQualifier = "" )
  : m_beginString( BeginString(beginString) ),
    m_senderCompID( SenderCompID(senderCompID) ),
    m_targetCompID( TargetCompID(targetCompID) ),
    m_sessionQualifier( sessionQualifier ),
	  m_isFIXT(false)
  {
    toString(m_frozenString);
		if( beginString.substr(0, 4) == "FIXT" )
			m_isFIXT = true;
  }

  const BeginString& getBeginString() const
    { return m_beginString; }
  const SenderCompID& getSenderCompID() const
    { return m_senderCompID; }
  const TargetCompID& getTargetCompID() const
    { return m_targetCompID; }
  const std::string& getSessionQualifier() const
    { return m_sessionQualifier; }
  const bool isFIXT() const
    { return m_isFIXT; }

  /// Get a string representation of the SessionID
  std::string toString() const
  {
    return m_frozenString;
  }
    
  // Return a reference for a high-performance scenario
  const std::string& toStringFrozen() const
  {
    return m_frozenString;
  }

  /// Build from string representation of SessionID
  void fromString( const std::string& str )
  {
    std::string::size_type first =
      str.find_first_of(':');
    std::string::size_type second =
      str.find("->");
    std::string::size_type third =
      str.find_last_of(':');
    if( first == std::string::npos )
      return;
    if( second == std::string::npos )
      return;
    m_beginString = str.substr(0, first);
    m_senderCompID = str.substr(first+1, second - first - 1);
    if( first == third )
    {
      m_targetCompID = str.substr(second+2);
      m_sessionQualifier = "";
    }
    else
    {
      m_targetCompID = str.substr(second+2, third - second - 2);
      m_sessionQualifier = str.substr(third+1);
    }
    toString(m_frozenString);
  }

  /// Get a string representation without making a copy
  std::string& toString( std::string& str ) const
  {
    str = getBeginString().getValue() + ":" +
          getSenderCompID().getValue() + "->" +
          getTargetCompID().getValue();
    if( m_sessionQualifier.size() )
      str += ":" + m_sessionQualifier;
    return str;
  }

  friend bool operator<( const SessionID&, const SessionID& );
  friend bool operator==( const SessionID&, const SessionID& );
  friend bool operator!=( const SessionID&, const SessionID& );
  friend std::ostream& operator<<( std::ostream&, const SessionID& );
  friend std::ostream& operator>>( std::ostream&, const SessionID& );

  SessionID operator~() const
  {
    return SessionID( m_beginString, SenderCompID( m_targetCompID ),
                      TargetCompID( m_senderCompID ), m_sessionQualifier );
  }

private:
  BeginString m_beginString;
  SenderCompID m_senderCompID;
  TargetCompID m_targetCompID;
  std::string m_sessionQualifier;
  bool m_isFIXT;
  std::string m_frozenString;
};
/*! @} */

inline bool operator<( const SessionID& lhs, const SessionID& rhs )
{
  return lhs.toStringFrozen() < rhs.toStringFrozen();
}

inline bool operator==( const SessionID& lhs, const SessionID& rhs )
{
  return lhs.toStringFrozen() == rhs.toStringFrozen();
}

inline bool operator!=( const SessionID& lhs, const SessionID& rhs )
{
  return !( lhs == rhs );
}

inline std::ostream& operator<<
( std::ostream& stream, const SessionID& sessionID )
{
  stream << sessionID.toStringFrozen();
  return stream;
}

inline std::istream& operator>>
( std::istream& stream, SessionID& sessionID )
{
  std::string str;
  stream >> str;
  sessionID.fromString( str );
  return stream;
}

}
#endif //FIX_SESSIONID_H

