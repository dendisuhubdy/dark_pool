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
#error PostgreSQLStore.h included, but HAVE_POSTGRESQL not defined
#endif

#ifdef HAVE_POSTGRESQL
#ifndef FIX_POSTGRESQLSTORE_H
#define FIX_POSTGRESQLSTORE_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "MessageStore.h"
#include "SessionSettings.h"
#include "PostgreSQLConnection.h"
#include <fstream>
#include <string>

namespace FIX
{
/// Creates a PostgreSQL based implementation of MessageStore.
class PostgreSQLStoreFactory : public MessageStoreFactory
{
public:
  static const std::string DEFAULT_DATABASE;
  static const std::string DEFAULT_USER;
  static const std::string DEFAULT_PASSWORD;
  static const std::string DEFAULT_HOST;
  static const short DEFAULT_PORT;

  PostgreSQLStoreFactory( const SessionSettings& settings )
: m_settings( settings ), m_useSettings( true ), m_useDictionary( false ) 
  {
    bool poolConnections = false;
    try { poolConnections = settings.get().getBool(POSTGRESQL_STORE_USECONNECTIONPOOL); }
    catch( ConfigError& ) {}

    m_connectionPoolPtr = PostgreSQLConnectionPoolPtr
      ( new PostgreSQLConnectionPool(poolConnections) );
  }

  PostgreSQLStoreFactory( const Dictionary& dictionary )
: m_dictionary( dictionary ), m_useSettings( false ), m_useDictionary( true ) 
  {
    m_connectionPoolPtr = PostgreSQLConnectionPoolPtr
      ( new PostgreSQLConnectionPool(false) );
  }

  PostgreSQLStoreFactory( const std::string& database, const std::string& user,
                          const std::string& password, const std::string& host,
                          short port )
: m_database( database ), m_user( user ), m_password( password ), m_host( host ), m_port( port ),
  m_useSettings( false ), m_useDictionary( false ) 
  {
    m_connectionPoolPtr = PostgreSQLConnectionPoolPtr
      ( new PostgreSQLConnectionPool(false) );
  }

  PostgreSQLStoreFactory()
: m_database( DEFAULT_DATABASE ), m_user( DEFAULT_USER ), m_password( DEFAULT_PASSWORD ),
  m_host( DEFAULT_HOST ), m_port( DEFAULT_PORT ), m_useSettings( false ), m_useDictionary( false ) 
  {
    m_connectionPoolPtr = PostgreSQLConnectionPoolPtr
      ( new PostgreSQLConnectionPool(false) );
  }

  MessageStore* create( const SessionID& );
  void destroy( MessageStore* );
private:
  MessageStore* create( const SessionID& s, const Dictionary& );

  PostgreSQLConnectionPoolPtr m_connectionPoolPtr;
  SessionSettings m_settings;
  Dictionary m_dictionary;
  std::string m_database;
  std::string m_user;
  std::string m_password;
  std::string m_host;
  short m_port;
  bool m_useSettings;
  bool m_useDictionary;
};
/*! @} */

/// PostgreSQL based implementation of MessageStore.
class PostgreSQLStore : public MessageStore
{
public:
  PostgreSQLStore( const SessionID& s, const DatabaseConnectionID& d, PostgreSQLConnectionPool* p );
  PostgreSQLStore( const SessionID& s, const std::string& database, const std::string& user,
                   const std::string& password, const std::string& host, short port );
  ~PostgreSQLStore();

  bool set( int, const std::string& ) throw ( IOException );
  void get( int, int, std::vector < std::string > & ) const throw ( IOException );

  int getNextSenderMsgSeqNum() const throw ( IOException );
  int getNextTargetMsgSeqNum() const throw ( IOException );
  void setNextSenderMsgSeqNum( int value ) throw ( IOException );
  void setNextTargetMsgSeqNum( int value ) throw ( IOException );
  void incrNextSenderMsgSeqNum() throw ( IOException );
  void incrNextTargetMsgSeqNum() throw ( IOException );

  UtcTimeStamp getCreationTime() const throw ( IOException );

  void reset() throw ( IOException );
  void refresh() throw ( IOException );

private:
  void populateCache();

  MemoryStore m_cache;
  PostgreSQLConnection* m_pConnection;
  PostgreSQLConnectionPool* m_pConnectionPool;
  SessionID m_sessionID;
};
}

#endif //FIX_POSTGRESQLSTORE_H
#endif //HAVE_POSTGRESQL
