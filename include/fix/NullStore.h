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

#ifndef FIX_NULLSTORE_H
#define FIX_NULLSTORE_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "MessageStore.h"
#include "SessionSettings.h"
#include <string>

namespace FIX
{
class Session;

/**
 * Null implementation of MessageStore.
 *
 * Will not actually store messages.  Useful for admin-only or market
 * data-only applications.
 */
class NullStoreFactory : public MessageStoreFactory
{
public:
  MessageStore* create( const SessionID& );
  void destroy( MessageStore* );
};
/*! @} */


/**
 * Null implementation of MessageStore.
 *
 * Will not actually store messages.  Useful for admin-only or market
 * data-only applications.
 */
class NullStore : public MessageStore
{
public:
  NullStore() : m_nextSenderMsgSeqNum( 1 ), m_nextTargetMsgSeqNum( 1 ) {}

  bool set( int, const std::string& ) throw ( IOException );
  void get( int, int, std::vector < std::string > & ) const throw ( IOException );

  int getNextSenderMsgSeqNum() const throw ( IOException )
  { return m_nextSenderMsgSeqNum; }
  int getNextTargetMsgSeqNum() const throw ( IOException )
  { return m_nextTargetMsgSeqNum; }
  void setNextSenderMsgSeqNum( int value ) throw ( IOException )
  { m_nextSenderMsgSeqNum = value; }
  void setNextTargetMsgSeqNum( int value ) throw ( IOException )
  { m_nextTargetMsgSeqNum = value; }
  void incrNextSenderMsgSeqNum() throw ( IOException )
  { ++m_nextSenderMsgSeqNum; }
  void incrNextTargetMsgSeqNum() throw ( IOException )
  { ++m_nextTargetMsgSeqNum; }

  void setCreationTime( const UtcTimeStamp& creationTime ) throw ( IOException )
  { m_creationTime = creationTime; }
  UtcTimeStamp getCreationTime() const throw ( IOException )
  { return m_creationTime; }

  void reset() throw ( IOException )
  {
    m_nextSenderMsgSeqNum = 1; m_nextTargetMsgSeqNum = 1;
    m_creationTime.setCurrent();
  }
  void refresh() throw ( IOException ) {}

private:
  int m_nextSenderMsgSeqNum;
  int m_nextTargetMsgSeqNum;
  UtcTimeStamp m_creationTime;
};
}

#endif // FIX_NULLSTORE_H

