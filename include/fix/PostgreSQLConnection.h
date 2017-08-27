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

#ifndef HAVE_POSTGRESQL
#error PostgreSQLConnection.h included, but HAVE_POSTGRESQL not defined
#endif

#ifdef HAVE_POSTGRESQL
#ifndef FIX_POSTGRESQLCONNECTION_H
#define FIX_POSTGRESQLCONNECTION_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#pragma comment( lib, "libpq" )
#endif

#include <libpq-fe.h>
#include "DatabaseConnectionID.h"
#include "DatabaseConnectionPool.h"
#include "Mutex.h"

namespace FIX
{
class PostgreSQLQuery
{
public:
  PostgreSQLQuery( const std::string& query ) 
  : m_result( 0 ), m_query( query ) 
  {}

  ~PostgreSQLQuery()
  {
    if( m_result )
      PQclear( m_result );
  }

  bool execute( PGconn* pConnection )
  {
    int retry = 0;
    
    do
    {
      if( m_result ) PQclear( m_result );
      m_result = PQexec( pConnection, m_query.c_str() );
      m_status = PQresultStatus( m_result );
      if( success() ) return true;
      PQreset( pConnection );
      retry++;
    } while( retry <= 1 );
    return success();
  }

  bool success()
  {
    return m_status == PGRES_TUPLES_OK
      || m_status == PGRES_COMMAND_OK;
  }

  int rows()
  {
    return PQntuples( m_result );
  }

  char* reason()
  {
    return PQresultErrorMessage( m_result );
  }

  char* getValue( int row, int column )
  {
    return PQgetvalue( m_result, row, column );
  }

  void throwException() throw( IOException )
  {
    if( !success() )
      throw IOException( "Query failed [" + m_query + "] " );
  }

private:
  PGresult* m_result;
  ExecStatusType m_status;
  std::string m_query; 
};

class PostgreSQLConnection
{
public:
  PostgreSQLConnection
  ( const DatabaseConnectionID& id )
  : m_connectionID( id )
  {
    connect();
  }

  PostgreSQLConnection
  ( const std::string& database, const std::string& user,
    const std::string& password, const std::string& host, short port )
  : m_connectionID( database, user, password, host, port )
  {
    connect();
  }

  ~PostgreSQLConnection()
  {
    if( m_pConnection )
      PQfinish( m_pConnection );
  }

  const DatabaseConnectionID& connectionID()
  {
    return m_connectionID;
  }

  bool connected()
  {
    Locker locker( m_mutex );
    return PQstatus( m_pConnection ) == CONNECTION_OK;
  }

  bool reconnect()
  {
    Locker locker( m_mutex );
    PQreset( m_pConnection );
    return connected();
  }

  bool execute( PostgreSQLQuery& pQuery )
  {
    Locker locker( m_mutex );
    return pQuery.execute( m_pConnection );
  }

private:
  void connect()
  {
    short port = m_connectionID.getPort();
    m_pConnection = PQsetdbLogin
      ( m_connectionID.getHost().c_str(), port == 0 ? "" : IntConvertor::convert( port ).c_str(),
        "", "", m_connectionID.getDatabase().c_str(), m_connectionID.getUser().c_str(), m_connectionID.getPassword().c_str() );

    if( !connected() )
      throw ConfigError( "Unable to connect to database" );
  }

  PGconn* m_pConnection;
  DatabaseConnectionID m_connectionID;
  Mutex m_mutex;
};

typedef DatabaseConnectionPool<PostgreSQLConnection>
  PostgreSQLConnectionPool;
typedef std::auto_ptr< PostgreSQLConnectionPool >
  PostgreSQLConnectionPoolPtr;
}

#endif //FIX_POSTGRESQLCONNECTION_H
#endif //HAVE_POSTGRESQL
