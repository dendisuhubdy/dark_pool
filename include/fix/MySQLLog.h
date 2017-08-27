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

#ifndef HAVE_MYSQL
#error MySQLLog.h included, but HAVE_MYSQL not defined
#endif

#ifdef HAVE_MYSQL
#ifndef FIX_MYSQLLOG_H
#define FIX_MYSQLLOG_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#pragma comment( lib, "libMySQL" )
#endif

#include "Log.h"
#include "SessionSettings.h"
#include "MySQLConnection.h"
#include <fstream>
#include <string>

namespace FIX
{
/// MySQL based implementation of Log.
class MySQLLog : public Log
{
public:
  MySQLLog( const SessionID& s, const DatabaseConnectionID& d, MySQLConnectionPool* p );
  MySQLLog( const DatabaseConnectionID& d, MySQLConnectionPool* p );
  MySQLLog( const SessionID& s, const std::string& database, const std::string& user,
            const std::string& password, const std::string& host, short port );
  MySQLLog( const std::string& database, const std::string& user,
            const std::string& password, const std::string& host, short port );

  ~MySQLLog();

  void clear();
  void backup();
  void setIncomingTable( const std::string& incomingTable )
  { m_incomingTable = incomingTable; }
  void setOutgoingTable( const std::string& outgoingTable )
  { m_outgoingTable = outgoingTable; }
  void setEventTable( const std::string& eventTable )
  { m_eventTable = eventTable; }

  void onIncoming( const std::string& value )
  { insert( m_incomingTable, value ); }
  void onOutgoing( const std::string& value )
  { insert( m_outgoingTable, value ); }
  void onEvent( const std::string& value )
  { insert( m_eventTable, value ); }

private:
  void init();
  void insert( const std::string& table, const std::string value );

  std::string m_incomingTable;
  std::string m_outgoingTable;
  std::string m_eventTable;
  MySQLConnection* m_pConnection;
  MySQLConnectionPool* m_pConnectionPool;
  SessionID* m_pSessionID;
};

/// Creates a MySQL based implementation of Log.
class MySQLLogFactory : public LogFactory
{
public:
  static const std::string DEFAULT_DATABASE;
  static const std::string DEFAULT_USER;
  static const std::string DEFAULT_PASSWORD;
  static const std::string DEFAULT_HOST;
  static const short DEFAULT_PORT;

  MySQLLogFactory( const SessionSettings& settings )
: m_settings( settings ), m_useSettings( true ) 
  {
    bool poolConnections = false;
    try { poolConnections = settings.get().getBool(MYSQL_LOG_USECONNECTIONPOOL); }
    catch( ConfigError& ) {}

    m_connectionPoolPtr = MySQLConnectionPoolPtr
      ( new MySQLConnectionPool(poolConnections) );
  }

  MySQLLogFactory( const std::string& database, const std::string& user,
                   const std::string& password, const std::string& host,
                   short port )
: m_database( database ), m_user( user ), m_password( password ), m_host( host ), m_port( port ),
  m_useSettings( false ) 
  {
    m_connectionPoolPtr = MySQLConnectionPoolPtr
      ( new MySQLConnectionPool(false) );
  }

  MySQLLogFactory()
: m_database( DEFAULT_DATABASE ), m_user( DEFAULT_USER ), m_password( DEFAULT_PASSWORD ),
  m_host( DEFAULT_HOST ), m_port( DEFAULT_PORT ), m_useSettings( false ) 
  {
    m_connectionPoolPtr = MySQLConnectionPoolPtr
      ( new MySQLConnectionPool(false) );
  }

  Log* create();
  Log* create( const SessionID& );
  void destroy( Log* );
private:
  void init( const Dictionary& settings, std::string& database,
             std::string& user, std::string& password,
             std::string& host, short& port );

  void initLog( const Dictionary& settings, MySQLLog& log );

  MySQLConnectionPoolPtr m_connectionPoolPtr;
  SessionSettings m_settings;
  std::string m_database;
  std::string m_user;
  std::string m_password;
  std::string m_host;
  short m_port;
  bool m_useSettings;
};
}

#endif //FIX_MYSQLLOG_H
#endif //HAVE_MYSQL
