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
#error OdbcLog.h included, but HAVE_ODBC not defined
#endif

#ifdef HAVE_ODBC
#ifndef FIX_ODBCLOG_H
#define FIX_ODBCLOG_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "OdbcConnection.h"
#include "Log.h"
#include "SessionSettings.h"
#include <fstream>
#include <string>

namespace FIX
{
/// ODBC based implementation of Log.
class OdbcLog : public Log
{
public:
  OdbcLog( const SessionID& s, const std::string& user, const std::string& password, 
           const std::string& connectionString );
  OdbcLog( const std::string& user, const std::string& password, 
           const std::string& connectionString );

  ~OdbcLog();

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
  OdbcConnection* m_pConnection;
  SessionID* m_pSessionID;
};

/// Creates a ODBC based implementation of Log.
class OdbcLogFactory : public LogFactory
{
public:
  static const std::string DEFAULT_USER;
  static const std::string DEFAULT_PASSWORD;
  static const std::string DEFAULT_CONNECTION_STRING;

  OdbcLogFactory( const SessionSettings& settings )
  : m_settings( settings ), m_useSettings( true ) {}

  OdbcLogFactory( const std::string& user, const std::string& password, 
                  const std::string& connectionString );

  OdbcLogFactory();

  ~OdbcLogFactory();

  Log* create();
  Log* create( const SessionID& );
  void destroy( Log* );
private:
  void init( const Dictionary& settings, 
             std::string& user, std::string& password, 
             std::string& connectionString );

  void initLog( const Dictionary& settings, OdbcLog& log );

  SessionSettings m_settings;
  std::string m_user;
  std::string m_password;
  std::string m_connectionString;
  bool m_useSettings;
};
}

#endif
#endif
