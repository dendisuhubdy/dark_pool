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

#ifndef FIX50SP2_CONFIRMATIONREQUEST_H
#define FIX50SP2_CONFIRMATIONREQUEST_H

#include "Message.h"

namespace FIX50SP2
{

  class ConfirmationRequest : public Message
  {
  public:
    ConfirmationRequest() : Message(MsgType()) {}
    ConfirmationRequest(const FIX::Message& m) : Message(m) {}
    ConfirmationRequest(const Message& m) : Message(m) {}
    ConfirmationRequest(const ConfirmationRequest& m) : Message(m) {}
    static FIX::MsgType MsgType() { return FIX::MsgType("BH"); }

    ConfirmationRequest(
      const FIX::ConfirmReqID& aConfirmReqID,
      const FIX::ConfirmType& aConfirmType,
      const FIX::TransactTime& aTransactTime )
    : Message(MsgType())
    {
      set(aConfirmReqID);
      set(aConfirmType);
      set(aTransactTime);
    }

    FIELD_SET(*this, FIX::ConfirmReqID);
    FIELD_SET(*this, FIX::ConfirmType);
    FIELD_SET(*this, FIX::NoOrders);
    class NoOrders: public FIX::Group
    {
    public:
    NoOrders() : FIX::Group(73,11,FIX::message_order(11,37,198,526,66,756,38,799,800,0)) {}
      FIELD_SET(*this, FIX::ClOrdID);
      FIELD_SET(*this, FIX::OrderID);
      FIELD_SET(*this, FIX::SecondaryOrderID);
      FIELD_SET(*this, FIX::SecondaryClOrdID);
      FIELD_SET(*this, FIX::ListID);
      FIELD_SET(*this, FIX::NoNested2PartyIDs);
      class NoNested2PartyIDs: public FIX::Group
      {
      public:
      NoNested2PartyIDs() : FIX::Group(756,757,FIX::message_order(757,758,759,806,0)) {}
        FIELD_SET(*this, FIX::Nested2PartyID);
        FIELD_SET(*this, FIX::Nested2PartyIDSource);
        FIELD_SET(*this, FIX::Nested2PartyRole);
        FIELD_SET(*this, FIX::NoNested2PartySubIDs);
        class NoNested2PartySubIDs: public FIX::Group
        {
        public:
        NoNested2PartySubIDs() : FIX::Group(806,760,FIX::message_order(760,807,0)) {}
          FIELD_SET(*this, FIX::Nested2PartySubID);
          FIELD_SET(*this, FIX::Nested2PartySubIDType);
        };
      };
      FIELD_SET(*this, FIX::OrderQty);
      FIELD_SET(*this, FIX::OrderAvgPx);
      FIELD_SET(*this, FIX::OrderBookingQty);
    };
    FIELD_SET(*this, FIX::AllocID);
    FIELD_SET(*this, FIX::SecondaryAllocID);
    FIELD_SET(*this, FIX::IndividualAllocID);
    FIELD_SET(*this, FIX::TransactTime);
    FIELD_SET(*this, FIX::AllocAccount);
    FIELD_SET(*this, FIX::AllocAcctIDSource);
    FIELD_SET(*this, FIX::AllocAccountType);
    FIELD_SET(*this, FIX::Text);
    FIELD_SET(*this, FIX::EncodedTextLen);
    FIELD_SET(*this, FIX::EncodedText);
  };

}

#endif
