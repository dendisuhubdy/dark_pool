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

#ifndef FIX_SOCKETCONNECTION_H
#define FIX_SOCKETCONNECTION_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "Parser.h"
#include "Responder.h"
#include "SessionID.h"
#include "SocketMonitor.h"
#include "Utility.h"
#include <set>

namespace FIX
{
class SocketAcceptor;
class SocketServer;
class SocketConnector;
class SocketInitiator;
class Session;

/// Encapsulates a socket file descriptor (single-threaded).
class SocketConnection : Responder
{
public:
  typedef std::set<SessionID> Sessions;

  SocketConnection( int s, Sessions sessions, SocketMonitor* pMonitor );
  SocketConnection( SocketInitiator&, const SessionID&, int, SocketMonitor* );
  virtual ~SocketConnection();

  int getSocket() const { return m_socket; }
  Session* getSession() const { return m_pSession; }

  bool read( SocketConnector& s );
  bool read( SocketAcceptor&, SocketServer& );
  bool processQueue();

  void signal()
  {
    Locker l( m_mutex );
    if( m_sendQueue.size() == 1 )
      m_pMonitor->signal( m_socket );
  }

  void unsignal()
  {
    Locker l( m_mutex );
    if( m_sendQueue.size() == 0 )
      m_pMonitor->unsignal( m_socket );
  }

  void onTimeout();

private:
  typedef std::deque<std::string, ALLOCATOR<std::string> >
    Queue;

  bool isValidSession();
  void readFromSocket() throw( SocketRecvFailed );
  bool readMessage( std::string& msg );
  void readMessages( SocketMonitor& s );
  bool send( const std::string& );
  void disconnect();

  int m_socket;
  char m_buffer[BUFSIZ];

  Parser m_parser;
  Queue m_sendQueue;
  unsigned m_sendLength;
  Sessions m_sessions;
  Session* m_pSession;
  SocketMonitor* m_pMonitor;
  Mutex m_mutex;
  fd_set m_fds;
};
}

#endif //FIX_SOCKETCONNECTION_H
