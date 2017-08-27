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

#ifndef FIX50SP2_LISTSTATUS_H
#define FIX50SP2_LISTSTATUS_H

#include "Message.h"

namespace FIX50SP2
{

  class ListStatus : public Message
  {
  public:
    ListStatus() : Message(MsgType()) {}
    ListStatus(const FIX::Message& m) : Message(m) {}
    ListStatus(const Message& m) : Message(m) {}
    ListStatus(const ListStatus& m) : Message(m) {}
    static FIX::MsgType MsgType() { return FIX::MsgType("N"); }

    ListStatus(
      const FIX::ListID& aListID,
      const FIX::ListStatusType& aListStatusType,
      const FIX::NoRpts& aNoRpts,
      const FIX::ListOrderStatus& aListOrderStatus,
      const FIX::RptSeq& aRptSeq,
      const FIX::TotNoOrders& aTotNoOrders )
    : Message(MsgType())
    {
      set(aListID);
      set(aListStatusType);
      set(aNoRpts);
      set(aListOrderStatus);
      set(aRptSeq);
      set(aTotNoOrders);
    }

    FIELD_SET(*this, FIX::ListID);
    FIELD_SET(*this, FIX::ListStatusType);
    FIELD_SET(*this, FIX::NoRpts);
    FIELD_SET(*this, FIX::ListOrderStatus);
    FIELD_SET(*this, FIX::RptSeq);
    FIELD_SET(*this, FIX::ListStatusText);
    FIELD_SET(*this, FIX::EncodedListStatusTextLen);
    FIELD_SET(*this, FIX::EncodedListStatusText);
    FIELD_SET(*this, FIX::TransactTime);
    FIELD_SET(*this, FIX::TotNoOrders);
    FIELD_SET(*this, FIX::LastFragment);
    FIELD_SET(*this, FIX::NoOrders);
    class NoOrders: public FIX::Group
    {
    public:
    NoOrders() : FIX::Group(73,11,FIX::message_order(11,526,14,39,636,151,84,6,103,58,354,355,37,0)) {}
      FIELD_SET(*this, FIX::ClOrdID);
      FIELD_SET(*this, FIX::SecondaryClOrdID);
      FIELD_SET(*this, FIX::CumQty);
      FIELD_SET(*this, FIX::OrdStatus);
      FIELD_SET(*this, FIX::WorkingIndicator);
      FIELD_SET(*this, FIX::LeavesQty);
      FIELD_SET(*this, FIX::CxlQty);
      FIELD_SET(*this, FIX::AvgPx);
      FIELD_SET(*this, FIX::OrdRejReason);
      FIELD_SET(*this, FIX::Text);
      FIELD_SET(*this, FIX::EncodedTextLen);
      FIELD_SET(*this, FIX::EncodedText);
      FIELD_SET(*this, FIX::OrderID);
    };
    FIELD_SET(*this, FIX::ContingencyType);
    FIELD_SET(*this, FIX::ListRejectReason);
  };

}

#endif
