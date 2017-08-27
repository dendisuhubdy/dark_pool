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

#ifndef FIX_FILELOG_H
#define FIX_FILELOG_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "Log.h"
#include "SessionSettings.h"
#include <fstream>

namespace FIX
{
/**
 * Creates a file based implementation of Log
 *
 * This stores all log events into flat files
 */
class FileLogFactory : public LogFactory
{
public:
  FileLogFactory( const SessionSettings& settings )
: m_settings( settings ), m_globalLog(0), m_globalLogCount(0) {};
  FileLogFactory( const std::string& path )
: m_path( path ), m_backupPath( path ), m_globalLog(0), m_globalLogCount(0) {};
  FileLogFactory( const std::string& path, const std::string& backupPath )
: m_path( path ), m_backupPath( backupPath ), m_globalLog(0), m_globalLogCount(0) {};

public:
  Log* create();
  Log* create( const SessionID& );
  void destroy( Log* log );

private:
  std::string m_path;
  std::string m_backupPath;
  SessionSettings m_settings;
  Log* m_globalLog;
  int m_globalLogCount;
};

/**
 * File based implementation of Log
 *
 * Two files are created by this implementation.  One for messages, 
 * and one for events.
 *
 */
class FileLog : public Log
{
public:
  FileLog( const std::string& path );
  FileLog( const std::string& path, const SessionID& sessionID );
  FileLog( const std::string& path, const std::string& backupPath, const SessionID& sessionID );
  virtual ~FileLog();

  void clear();
  void backup();

  void onIncoming( const std::string& value )
  { m_messages << UtcTimeStampConvertor::convert(UtcTimeStamp(), m_millisecondsInTimeStamp) << " : " << value << std::endl; }
  void onOutgoing( const std::string& value )
  { m_messages << UtcTimeStampConvertor::convert(UtcTimeStamp(), m_millisecondsInTimeStamp) << " : " << value << std::endl; }
  void onEvent( const std::string& value )
  {
    UtcTimeStamp now;
    m_event << UtcTimeStampConvertor::convert( now, m_millisecondsInTimeStamp )
            << " : " << value << std::endl;
  }

  bool getMillisecondsInTimeStamp() const
  { return m_millisecondsInTimeStamp; }
  void setMillisecondsInTimeStamp ( bool value )
  { m_millisecondsInTimeStamp = value; }

private:
  std::string generatePrefix( const SessionID& sessionID );
  void init( std::string path, std::string backupPath, const std::string& prefix );

  std::ofstream m_messages;
  std::ofstream m_event;
  std::string m_messagesFileName;
  std::string m_eventFileName;
  std::string m_fullPrefix;
  std::string m_fullBackupPrefix;
  bool m_millisecondsInTimeStamp;
};
}

#endif //FIX_LOG_H
