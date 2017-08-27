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

#ifndef FIX_HTTPCONNECTION_H
#define FIX_HTTPCONNECTION_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "HttpParser.h"
#include <stdio.h>

namespace FIX
{
class HttpMessage;

/// Encapsulates a HTTP socket file descriptor
class HttpConnection
{
public:
  HttpConnection( int s );

  int getSocket() const { return m_socket; }
  bool read();

private:
  bool readMessage( std::string& msg ) throw( SocketRecvFailed );
  void processStream();
  void processRequest( const HttpMessage& );
  void processRoot( const HttpMessage&, std::stringstream& h, std::stringstream& b );
  void processResetSessions( const HttpMessage&, std::stringstream& h, std::stringstream& b );
  void processRefreshSessions( const HttpMessage&, std::stringstream& h, std::stringstream& b );
  void processEnableSessions( const HttpMessage&, std::stringstream& h, std::stringstream& b );
  void processDisableSessions( const HttpMessage&, std::stringstream& h, std::stringstream& b );
  void processSession( const HttpMessage&, std::stringstream& h, std::stringstream& b );
  void processResetSession( const HttpMessage&, std::stringstream& h, std::stringstream& b );
  void processRefreshSession( const HttpMessage&, std::stringstream& h, std::stringstream& b );
 
 void showToggle
    ( std::stringstream& s, const std::string& name, bool value, const std::string& url );
  void showRow
    ( std::stringstream& s, const std::string& name, bool value, const std::string& url = "" );
  void showRow
    ( std::stringstream& s, const std::string& name, const std::string& value, const std::string& url = "" );
  void showRow
    ( std::stringstream& s, const std::string& name, int value, const std::string& url = "" );

  bool send( const std::string& );
  void disconnect( int error = 0 );

  int m_socket;
  char m_buffer[BUFSIZ];

  HttpParser m_parser;
  fd_set m_fds;
};
}

#endif
