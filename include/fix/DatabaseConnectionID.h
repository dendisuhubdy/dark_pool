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

#ifndef FIX_DATABASECONNECTIONID_H
#define FIX_DATABASECONNECTIONID_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include <string>
#include <map>

namespace FIX
{
class DatabaseConnectionID
{
public:
  DatabaseConnectionID
  ( const std::string& database, const std::string& user,
    const std::string& password, const std::string& host, short port )
  : m_database( database ), m_user( user ), m_password( password ),
    m_host( host ), m_port( port ) {}

  friend bool operator<( const DatabaseConnectionID&, const DatabaseConnectionID& );
  friend bool operator==( const DatabaseConnectionID&, const DatabaseConnectionID& );
  friend bool operator!=( const DatabaseConnectionID&, const DatabaseConnectionID& );

  const std::string& getDatabase() const
    { return m_database; }
  const std::string& getUser() const
    { return m_user; }
  const std::string& getPassword() const
    { return m_password; }
  const std::string& getHost() const
    { return m_host; }
  short getPort() const
    { return m_port; }

private:
  std::string m_database;
  std::string m_user;
  std::string m_password;
  std::string m_host;
  short m_port;
};

inline bool operator<( const DatabaseConnectionID& lhs, const DatabaseConnectionID& rhs )
{
  if ( lhs.m_database < rhs.m_database )
    return true;
  else if ( rhs.m_database < lhs.m_database )
    return false;
  else if ( lhs.m_user < rhs.m_user )
    return true;
  else if ( rhs.m_user < lhs.m_user )
    return false;
  else if ( lhs.m_password < rhs.m_password )
    return true;
  else if ( rhs.m_password < lhs.m_password )
    return false;
  else if ( lhs.m_host < rhs.m_host )
    return true;
  else if ( rhs.m_host < lhs.m_host )
    return false;
  else if ( lhs.m_port < rhs.m_port )
    return true;
  else if ( rhs.m_port < lhs.m_port )
    return false;
  else
    return false;
}
inline bool operator==( const DatabaseConnectionID& lhs, const DatabaseConnectionID& rhs )
{
  return ( ( lhs.m_database == rhs.m_database ) &&
           ( lhs.m_user == rhs.m_user ) &&
           ( lhs.m_password == rhs.m_password ) &&
           ( lhs.m_host == rhs.m_host ) &&
           ( lhs.m_port == rhs.m_port ));
}
inline bool operator!=( const DatabaseConnectionID& lhs, const DatabaseConnectionID& rhs )
{
  return !( lhs == rhs );
}
}

#endif
