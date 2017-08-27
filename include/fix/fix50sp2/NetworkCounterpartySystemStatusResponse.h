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

#ifndef FIX50SP2_NETWORKCOUNTERPARTYSYSTEMSTATUSRESPONSE_H
#define FIX50SP2_NETWORKCOUNTERPARTYSYSTEMSTATUSRESPONSE_H

#include "Message.h"

namespace FIX50SP2
{

  class NetworkCounterpartySystemStatusResponse : public Message
  {
  public:
    NetworkCounterpartySystemStatusResponse() : Message(MsgType()) {}
    NetworkCounterpartySystemStatusResponse(const FIX::Message& m) : Message(m) {}
    NetworkCounterpartySystemStatusResponse(const Message& m) : Message(m) {}
    NetworkCounterpartySystemStatusResponse(const NetworkCounterpartySystemStatusResponse& m) : Message(m) {}
    static FIX::MsgType MsgType() { return FIX::MsgType("BD"); }

    NetworkCounterpartySystemStatusResponse(
      const FIX::NetworkStatusResponseType& aNetworkStatusResponseType,
      const FIX::NetworkResponseID& aNetworkResponseID )
    : Message(MsgType())
    {
      set(aNetworkStatusResponseType);
      set(aNetworkResponseID);
    }

    FIELD_SET(*this, FIX::NetworkStatusResponseType);
    FIELD_SET(*this, FIX::NetworkRequestID);
    FIELD_SET(*this, FIX::NetworkResponseID);
    FIELD_SET(*this, FIX::LastNetworkResponseID);
    FIELD_SET(*this, FIX::NoCompIDs);
    class NoCompIDs: public FIX::Group
    {
    public:
    NoCompIDs() : FIX::Group(936,930,FIX::message_order(930,931,283,284,928,929,0)) {}
      FIELD_SET(*this, FIX::RefCompID);
      FIELD_SET(*this, FIX::RefSubID);
      FIELD_SET(*this, FIX::LocationID);
      FIELD_SET(*this, FIX::DeskID);
      FIELD_SET(*this, FIX::StatusValue);
      FIELD_SET(*this, FIX::StatusText);
    };
  };

}

#endif
