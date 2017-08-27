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

#ifndef FIX_LOG_H
#define FIX_LOG_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "Message.h"
#include "Mutex.h"
#include "SessionSettings.h"
#include <map>
#include <vector>

namespace FIX
{
class Log;

/**
 * This interface must be implemented to create a Log.
 */
class LogFactory
{
public:
  virtual ~LogFactory() {}
  virtual Log* create() = 0;
  virtual Log* create( const SessionID& ) = 0;
  virtual void destroy( Log* ) = 0;
};

/**
 * Creates a screen based implementation of Log.
 *
 * This displays all log events onto the standard output
 */
class ScreenLogFactory : public LogFactory
{
public:
  ScreenLogFactory( const SessionSettings& settings )
: m_useSettings( true ), m_settings( settings ) {};
  ScreenLogFactory( bool incoming, bool outgoing, bool event )
: m_incoming( incoming ), m_outgoing( outgoing ), m_event( event ), m_useSettings( false ) {}

  Log* create();
  Log* create( const SessionID& );
  void destroy( Log* log );

private:
  void init( const Dictionary& settings, bool& incoming, bool& outgoing, bool& event );

  bool m_incoming;
  bool m_outgoing;
  bool m_event;
  bool m_useSettings;
  SessionSettings m_settings;
};

/**
 * This interface must be implemented to log messages and events
 */
class Log
{
public:
  virtual ~Log() {}

  virtual void clear() = 0;
  virtual void backup() = 0;
  virtual void onIncoming( const std::string& ) = 0;
  virtual void onOutgoing( const std::string& ) = 0;
  virtual void onEvent( const std::string& ) = 0;
};
/*! @} */

/**
 * Null implementation of Log
 *
 * This is only for internal use. Used when no log factory is
 * passed to the initiator or acceptor.
 */
class NullLog : public Log
{
public:
  void clear() {}
  void backup() {}
  void onIncoming( const std::string& ) {}
  void onOutgoing( const std::string& ) {}
  void onEvent( const std::string& ) {}
};

/**
 * Screen based implementation of Log.
 *
 * This will display all log information onto the standard output
 */
class ScreenLog : public Log
{
public:
  ScreenLog( bool incoming, bool outgoing, bool event ) 
: m_prefix( "GLOBAL" ),
  m_incoming( incoming ), m_outgoing( outgoing ), m_event( event ), m_millisecondsInTimeStamp( true ) {}

  ScreenLog( const SessionID& sessionID,
             bool incoming, bool outgoing, bool event )
: m_prefix( sessionID.toString() ),
  m_incoming( incoming ), m_outgoing( outgoing ), m_event( event ), m_millisecondsInTimeStamp( true ) {}

  void clear() {}
  void backup() {}

  void onIncoming( const std::string& value )
  {
    if ( !m_incoming ) return ;
    Locker l( s_mutex );
    m_time.setCurrent();
    std::cout << "<" << UtcTimeStampConvertor::convert(m_time, m_millisecondsInTimeStamp)
              << ", " << m_prefix
              << ", " << "incoming>" << std::endl
              << "  (" << value << ")" << std::endl;
  }

  void onOutgoing( const std::string& value )
  {
    if ( !m_outgoing ) return ;
    Locker l( s_mutex );
    m_time.setCurrent();
    std::cout << "<" << UtcTimeStampConvertor::convert(m_time, m_millisecondsInTimeStamp)
              << ", " << m_prefix
              << ", " << "outgoing>" << std::endl
              << "  (" << value << ")" << std::endl;
  }

  void onEvent( const std::string& value )
  {
    if ( !m_event ) return ;
    Locker l( s_mutex );
    m_time.setCurrent();
    std::cout << "<" << UtcTimeStampConvertor::convert(m_time, m_millisecondsInTimeStamp)
              << ", " << m_prefix
              << ", " << "event>" << std::endl
              << "  (" << value << ")" << std::endl;
  }

  bool getMillisecondsInTimeStamp() const
  { return m_millisecondsInTimeStamp; }
  void setMillisecondsInTimeStamp ( bool value )
  { m_millisecondsInTimeStamp = value; }

private:
  std::string m_prefix;
  UtcTimeStamp m_time;
  bool m_incoming;
  bool m_outgoing;
  bool m_event;
  static Mutex s_mutex;
  bool m_millisecondsInTimeStamp;
};
}

#endif //FIX_LOG_H
