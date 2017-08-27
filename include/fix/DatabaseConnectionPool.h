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

#ifndef FIX_DATABASECONNECTIONPOOL_H
#define FIX_DATABASECONNECTIONPOOL_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "DatabaseConnectionID.h"
#include <string>
#include <map>

namespace FIX
{
template< typename T > class DatabaseConnectionPool
{
public:
  DatabaseConnectionPool( bool poolConnections )
  : m_poolConnections( poolConnections ) {}

  T* create( const DatabaseConnectionID& id )
  {
    if( !m_poolConnections )
      return new T( id );

    if( m_connections.find( id ) == m_connections.end() )
    {
      m_connections[id] = Connection
        ( new T(id), 0 );
    }
    m_connections[id].second++;
    return m_connections[id].first;
  }

  bool destroy( T* pConnection )
  {
    if( !m_poolConnections )
    {
      delete pConnection;
      return true;
    }

    const DatabaseConnectionID& id = pConnection->connectionID();
    if( m_connections.find( id ) == m_connections.end() )
      return false;

    Connection& connection = m_connections[id];
    if( connection.first != pConnection )
      return false;

    connection.second--;
    if( connection.second == 0 )
    {
      m_connections.erase( id );
      delete pConnection;
    }
    return true;
  }

private:
  typedef std::pair<T*, int>
    Connection;
  typedef std::map<DatabaseConnectionID, Connection>
    Connections;

  Connections m_connections;
  bool m_poolConnections;
};
}

#endif
