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

#ifndef HAVE_ODBC
#error OdbcConnection.h included, but HAVE_ODBC not defined
#endif

#ifdef HAVE_ODBC
#ifndef FIX_ODBCCONNECTION_H
#define FIX_ODBCCONNECTION_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#pragma comment( lib, "Odbc32" )
#endif

#include "Utility.h"
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sstream>
#include "DatabaseConnectionID.h"
#include "DatabaseConnectionPool.h"
#include "Exceptions.h"
#include "Mutex.h"

namespace FIX
{

inline std::string odbcError( SQLSMALLINT statementType, SQLHANDLE handle )
{
  SQLCHAR state[6];
  SQLINTEGER error;
  SQLCHAR text[SQL_MAX_MESSAGE_LENGTH];
  SQLSMALLINT textLength;
  RETCODE result = SQLGetDiagRec
    ( statementType, handle, 1, state, &error, text, sizeof(text), &textLength );
  if( result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO )
    return std::string( (char*)text );
  return "";
}

inline bool odbcSuccess( RETCODE result )
{
  return result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO;
}

class OdbcQuery
{
public:
  OdbcQuery( const std::string& query ) 
  : m_statement( 0 ), m_result( 0 ), m_query( query ) 
  {}

  ~OdbcQuery()
  {
    close();
  }

  void close()
  {
    if( m_statement )
    {
      SQLFreeHandle( SQL_HANDLE_STMT, m_statement );
      m_statement = 0;
      m_result = 0;
    }
  }

  bool execute( HDBC connection )
  {
    if( m_statement ) SQLFreeHandle( SQL_HANDLE_STMT, m_statement );
    SQLAllocHandle( SQL_HANDLE_STMT, connection, &m_statement );
    m_result = SQLExecDirect( m_statement, (SQLCHAR*)m_query.c_str(), m_query.size() );
    if( success() || m_result == SQL_NO_DATA )
      return true;
    m_reason = odbcError( SQL_HANDLE_STMT, m_statement );
    SQLFreeHandle( SQL_HANDLE_STMT, m_statement );
    m_statement = 0;
    return success();
  }

  bool success()
  {
    return odbcSuccess( m_result );
  }

  /*int rows()
  {
    return (int)mysql_num_rows( m_result );
  }*/

  const std::string& reason()
  {
    return m_reason;
  }

  bool fetch()
  {
    return odbcSuccess( SQLFetch(m_statement) );
  }

  /*char* getValue( int row, int column )
  {
    if( m_rows.empty() )
    {
      MYSQL_ROW row = 0;
      while( row = mysql_fetch_row( m_result ) )
        m_rows.push_back(row);
    }
    return m_rows[row][column];
  }*/

  HSTMT statement()
  {
    return m_statement;
  }

  void throwException() throw( IOException )
  {
    if( !success() )
      throw IOException( "Query failed [" + m_query + "] " + reason() );
  }

private:
  HSTMT m_statement;
  RETCODE m_result;
  std::string m_query; 
  std::string m_reason;
};

class OdbcConnection
{
public:
  OdbcConnection
  ( const DatabaseConnectionID& id )
    : m_connection( 0 ), m_environment( 0 ), m_connectionID( id )
  {
    connect();
  }

  OdbcConnection
  ( const std::string& user, const std::string& password, 
    const std::string& connectionString )
  : m_connection( 0 ), m_environment( 0 ), m_connectionID( "", user, password, connectionString, 0 )
  {
    connect();
  }

  ~OdbcConnection()
  {
    if( m_connection )
    {
      SQLDisconnect( m_connection );
      SQLFreeHandle( SQL_HANDLE_DBC, m_connection );
      SQLFreeHandle( SQL_HANDLE_ENV, m_environment );
    }
  }

  const DatabaseConnectionID& connectionID()
  {
    return m_connectionID;
  }

  bool connected()
  {
    Locker locker( m_mutex );
    return m_connected;
  }

  bool reconnect()
  {
    Locker locker( m_mutex );
    SQLDisconnect( m_connection );
    SQLFreeHandle( SQL_HANDLE_DBC, m_connection );
    m_connection = 0;
    connect();
    return true;
  }

  bool execute( OdbcQuery& pQuery )
  {
    Locker locker( m_mutex );
    if( !pQuery.execute( m_connection ) )
    {
      reconnect();
      return pQuery.execute( m_connection );
    }
    return true;
  }

private:
  void connect()
  {
    m_connected = false;

    RETCODE result;
    if(!m_environment)
    {
      result = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_environment );
      if( !odbcSuccess(result) )
      throw ConfigError( "Unable to allocate ODBC environment" );
	    
      result = SQLSetEnvAttr(m_environment, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
      if( !odbcSuccess(result) )
        throw ConfigError( "Unable to find ODBC version 3.0" );
    }

    result = SQLAllocHandle( SQL_HANDLE_DBC, m_environment, &m_connection );
    if( !odbcSuccess(result) )
      throw ConfigError( "Unable to allocate ODBC connection" );

    std::stringstream connectionStream;
    std::string connectionString = m_connectionID.getHost();
    if( m_connectionID.getHost().find("UID=") == std::string::npos )
      connectionStream << "UID=" << m_connectionID.getUser() << ";";
    if( m_connectionID.getHost().find("PWD=") == std::string::npos )
      connectionStream << "PWD=" << m_connectionID.getPassword() << ";";
    connectionStream << m_connectionID.getHost();
    connectionString = connectionStream.str();

    SQLCHAR connectionStringOut[255];

    result = SQLDriverConnect(
      m_connection, NULL,
      (SQLCHAR*)connectionString.c_str(), SQL_NTS,
      connectionStringOut, 255,
      0, SQL_DRIVER_NOPROMPT );

    if( !odbcSuccess(result) )
    {
      std::string error = odbcError( SQL_HANDLE_DBC, m_connection );
      throw ConfigError( error );
    }

    m_connected = true;
  }

  HENV m_environment;
  HDBC m_connection;
  bool m_connected;
  DatabaseConnectionID m_connectionID;
  Mutex m_mutex;
};
}

#endif
#endif
