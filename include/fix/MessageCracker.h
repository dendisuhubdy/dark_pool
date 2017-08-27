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

#ifndef FIX_MESSAGECRACKER_H
#define FIX_MESSAGECRACKER_H

#include "fix40/MessageCracker.h"
#include "fix41/MessageCracker.h"
#include "fix42/MessageCracker.h"
#include "fix43/MessageCracker.h"
#include "fix44/MessageCracker.h"
#include "fix50/MessageCracker.h"
#include "fixt11/MessageCracker.h"
#include "Values.h"
#include "Session.h"

namespace FIX
{
/** Takes in a generic Message and produces an object that represents
 *  its specific version and message type.
 */
class MessageCracker
      : 
      public FIX40::MessageCracker,
      public FIX41::MessageCracker,
      public FIX42::MessageCracker,
      public FIX43::MessageCracker,
      public FIX44::MessageCracker,
      public FIX50::MessageCracker,
      public FIXT11::MessageCracker
{
public:
  void crack( const Message& message,
              const SessionID& sessionID )
  {
    const FIX::BeginString& beginString = 
      FIELD_GET_REF( message.getHeader(), BeginString );

    crack( message, sessionID, beginString );
  }

  void crack( const Message& message,
              const SessionID& sessionID,
              const BeginString& beginString )
  {
    if ( beginString == BeginString_FIX40 )
      ((FIX40::MessageCracker&)(*this)).crack((const FIX40::Message&) message, sessionID);
    else if ( beginString == BeginString_FIX41 )
      ((FIX41::MessageCracker&)(*this)).crack((const FIX41::Message&) message, sessionID);
    else if ( beginString == BeginString_FIX42 )
      ((FIX42::MessageCracker&)(*this)).crack((const FIX42::Message&) message, sessionID);
    else if ( beginString == BeginString_FIX43 )
      ((FIX43::MessageCracker&)(*this)).crack((const FIX43::Message&) message, sessionID);
    else if ( beginString == BeginString_FIX44 )
      ((FIX44::MessageCracker&)(*this)).crack((const FIX44::Message&) message, sessionID);
    else if ( beginString == BeginString_FIX50 )
      ((FIX50::MessageCracker&)(*this)).crack((const FIX50::Message&) message, sessionID);
    else if ( beginString == BeginString_FIXT11 )
    {
      if( message.isAdmin() )
      {
        ((FIXT11::MessageCracker&)(*this)).crack((const FIXT11::Message&) message, sessionID);
      }
      else
      {
        ApplVerID applVerID;
        if(!message.getHeader().isSetField(applVerID))
        {
          Session* pSession = Session::lookupSession( sessionID );
          applVerID = pSession->getSenderDefaultApplVerID();
        }
        else
        {
          message.getHeader().getField( applVerID );
        }

        crack( message, sessionID, Message::toBeginString(applVerID) );
      }
    }
  }

  void crack( Message& message,
              const SessionID& sessionID )
  {
    const FIX::BeginString& beginString = 
      FIELD_GET_REF( message.getHeader(), BeginString );

    crack( message, sessionID, beginString );
  }

  void crack( Message& message,
              const SessionID& sessionID,
              const BeginString& beginString )
  {
    if ( beginString == BeginString_FIX40 )
      ((FIX40::MessageCracker&)(*this)).crack((const FIX40::Message&) message, sessionID);
    else if ( beginString == BeginString_FIX41 )
      ((FIX41::MessageCracker&)(*this)).crack((const FIX41::Message&) message, sessionID);
    else if ( beginString == BeginString_FIX42 )
      ((FIX42::MessageCracker&)(*this)).crack((const FIX42::Message&) message, sessionID);
    else if ( beginString == BeginString_FIX43 )
      ((FIX43::MessageCracker&)(*this)).crack((const FIX43::Message&) message, sessionID);
    else if ( beginString == BeginString_FIX44 )
      ((FIX44::MessageCracker&)(*this)).crack((const FIX44::Message&) message, sessionID);
    else if ( beginString == BeginString_FIX50 )
      ((FIX50::MessageCracker&)(*this)).crack((const FIX50::Message&) message, sessionID);
    else if ( beginString == BeginString_FIXT11 )
    {
      if( message.isAdmin() )
      {
        ((FIXT11::MessageCracker&)(*this)).crack((const FIXT11::Message&) message, sessionID);
      }
      else
      {
        ApplVerID applVerID;
        if(!message.getHeader().isSetField(applVerID))
        {
          Session* pSession = Session::lookupSession( sessionID );
          applVerID = pSession->getSenderDefaultApplVerID();
        }
        else
        {
          message.getHeader().getField( applVerID );
        }

        crack( message, sessionID, Message::toBeginString(applVerID) );
      }
    }
  }
};
}

#endif //FIX_MESSAGECRACKER_H
