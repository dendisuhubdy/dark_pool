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

#ifndef FIX_SOCKETSERVER_H
#define FIX_SOCKETSERVER_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "SocketMonitor.h"
#include "Exceptions.h"
#include <map>
#include <set>
#include <queue>

namespace FIX
{
/// Information about listening socket
struct SocketInfo
{
  SocketInfo()
  : m_socket( -1 ), m_port( 0 ), m_noDelay( false ),
    m_sendBufSize( 0 ), m_rcvBufSize( 0 ) {}
  
  SocketInfo( int socket, short port, bool noDelay, int sendBufSize, int rcvBufSize )
  : m_socket( socket ), m_port( port ), m_noDelay( noDelay ), 
    m_sendBufSize( sendBufSize ), m_rcvBufSize( rcvBufSize ) {}

  int m_socket;
  short m_port;
  bool m_noDelay;
  int m_sendBufSize;
  int m_rcvBufSize;
};

/// Listens for and accepts incoming socket connections on a port.
class SocketServer
{
public:
  class Strategy;

  SocketServer( int timeout = 0 );

  int add( int port, bool reuse = false, bool noDelay = false, 
           int sendBufSize = 0, int rcvBufSize = 0 ) throw( SocketException& );
  int accept( int socket );
  void close();
  bool block( Strategy& strategy, bool poll = 0, double timeout = 0.0 );

  int numConnections() { return m_monitor.numSockets() - 1; }
  SocketMonitor& getMonitor() { return m_monitor; }

  int socketToPort( int socket );
  int portToSocket( int port );

private:
  typedef std::map<int, SocketInfo>
    SocketToInfo;
  typedef std::map<int, SocketInfo>
    PortToInfo;

  SocketToInfo m_socketToInfo;  
  PortToInfo m_portToInfo;
  SocketMonitor m_monitor;

public:
  class Strategy
  {
  public:
    virtual ~Strategy() {}
    virtual void onConnect( SocketServer&, int acceptSocket, int socket ) = 0;
    virtual void onWrite( SocketServer&, int socket ) = 0;
    virtual bool onData( SocketServer&, int socket ) = 0;
    virtual void onDisconnect( SocketServer&, int socket ) = 0;
    virtual void onError( SocketServer& ) = 0;
    virtual void onTimeout( SocketServer& ) {};
  };
};
}

#endif //FIX_SOCKETSERVER_H
