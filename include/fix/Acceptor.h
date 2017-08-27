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

#ifndef FIX_ACCEPTOR_H
#define FIX_ACCEPTOR_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "Application.h"
#include "MessageStore.h"
#include "Log.h"
#include "Responder.h"
#include "SessionSettings.h"
#include "Exceptions.h"
#include <map>
#include <string>

namespace FIX
{
class Client;
class Session;

/**
 * Base for classes which act as an acceptor for incoming connections.
 *
 * Most users will not need to implement one of these.  The default
 * SocketAcceptor implementation will be used in most cases.
 */
class Acceptor
{
public:
  Acceptor( Application&, MessageStoreFactory&,
            const SessionSettings& ) throw( ConfigError );
  Acceptor( Application&, MessageStoreFactory&,
            const SessionSettings&, LogFactory& ) throw( ConfigError );

  virtual ~Acceptor();

  Log* getLog() 
  { 
    if( m_pLog ) return m_pLog;
    return &m_nullLog;
  }

  /// Start acceptor.
  void start() throw ( ConfigError, RuntimeError );
  /// Block on the acceptor
  void block() throw ( ConfigError, RuntimeError );
  /// Poll the acceptor
  bool poll( double timeout = 0.0 ) throw ( ConfigError, RuntimeError );

  /// Stop acceptor.
  void stop( bool force = false );

  /// Check to see if any sessions are currently logged on
  bool isLoggedOn();

  Session* getSession( const std::string& msg, Responder& );

  const std::set<SessionID>& getSessions() const { return m_sessionIDs; }
  Session* getSession( const SessionID& sessionID ) const;
  const Dictionary* const getSessionSettings( const SessionID& sessionID ) const;

  bool has( const SessionID& id )
  { return m_sessions.find( id ) != m_sessions.end(); }

  bool isStopped() { return m_stop; }

  Application& getApplication() { return m_application; }
  MessageStoreFactory& getMessageStoreFactory()
  { return m_messageStoreFactory; }

private:
  void initialize() throw ( ConfigError );

  /// Implemented to configure acceptor
  virtual void onConfigure( const SessionSettings& ) throw ( ConfigError ) {};
  /// Implemented to initialize acceptor
  virtual void onInitialize( const SessionSettings& ) throw ( RuntimeError ) {};
  /// Implemented to start listening for connections.
  virtual void onStart() = 0;
  /// Implemented to connect and poll for events.
  virtual bool onPoll( double second ) = 0;
  /// Implemented to stop a running acceptor.
  virtual void onStop() = 0;

  static THREAD_PROC startThread( void* p );

  typedef std::set < SessionID > SessionIDs;
  typedef std::map < SessionID, Session* > Sessions;

  thread_id m_threadid;
  Sessions m_sessions;
  SessionIDs m_sessionIDs;
  Application& m_application;
  MessageStoreFactory& m_messageStoreFactory;
  SessionSettings m_settings;
  LogFactory* m_pLogFactory;
  Log* m_pLog;
  NullLog m_nullLog;
  bool m_firstPoll;
  bool m_stop;
};
/*! @} */
}

#endif // FIX_ACCEPTOR_H
