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

#ifndef FIX_THREADEDSOCKETACCEPTOR_H
#define FIX_THREADEDSOCKETACCEPTOR_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "Acceptor.h"
#include "ThreadedSocketConnection.h"
#include "Mutex.h"

namespace FIX
{
/// Threaded Socket implementation of Acceptor.
class ThreadedSocketAcceptor : public Acceptor
{
  friend class SocketConnection;
public:
  ThreadedSocketAcceptor( Application&, MessageStoreFactory&,
                          const SessionSettings& ) throw( ConfigError );
  ThreadedSocketAcceptor( Application&, MessageStoreFactory&,
                          const SessionSettings&,
                          LogFactory& ) throw( ConfigError );

  virtual ~ThreadedSocketAcceptor();

private:
  struct AcceptorThreadInfo
  {
    AcceptorThreadInfo( ThreadedSocketAcceptor* pAcceptor, int socket, int port )
    : m_pAcceptor( pAcceptor ), m_socket( socket ), m_port( port ) {}

    ThreadedSocketAcceptor* m_pAcceptor;
    int m_socket;
    int m_port;
  };

  struct ConnectionThreadInfo
  {
    ConnectionThreadInfo( ThreadedSocketAcceptor* pAcceptor, 
                          ThreadedSocketConnection* pConnection )
    : m_pAcceptor( pAcceptor ), m_pConnection( pConnection ) {}

    ThreadedSocketAcceptor* m_pAcceptor;
    ThreadedSocketConnection* m_pConnection;
  };

  bool readSettings( const SessionSettings& );

  typedef std::set < int >  Sockets;
  typedef std::set < SessionID > Sessions;
  typedef std::map < int, Sessions > PortToSessions;
  typedef std::map < int, int > SocketToPort;
  typedef std::map < int, thread_id > SocketToThread;

  void onConfigure( const SessionSettings& ) throw ( ConfigError );
  void onInitialize( const SessionSettings& ) throw ( RuntimeError );

  void onStart();
  bool onPoll( double timeout );
  void onStop();

  void addThread( int s, thread_id t );
  void removeThread( int s );
  static THREAD_PROC socketAcceptorThread( void* p );
  static THREAD_PROC socketConnectionThread( void* p );

  Sockets m_sockets;
  PortToSessions m_portToSessions;
  SocketToPort m_socketToPort;
  SocketToThread m_threads;
  Mutex m_mutex;
};
/*! @} */
}

#endif //FIX_THREADEDSOCKETACCEPTOR_H
