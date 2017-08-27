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

#ifndef FIX50SP2_BIDRESPONSE_H
#define FIX50SP2_BIDRESPONSE_H

#include "Message.h"

namespace FIX50SP2
{

  class BidResponse : public Message
  {
  public:
    BidResponse() : Message(MsgType()) {}
    BidResponse(const FIX::Message& m) : Message(m) {}
    BidResponse(const Message& m) : Message(m) {}
    BidResponse(const BidResponse& m) : Message(m) {}
    static FIX::MsgType MsgType() { return FIX::MsgType("l"); }

    FIELD_SET(*this, FIX::BidID);
    FIELD_SET(*this, FIX::ClientBidID);
    FIELD_SET(*this, FIX::NoBidComponents);
    class NoBidComponents: public FIX::Group
    {
    public:
    NoBidComponents() : FIX::Group(420,12,FIX::message_order(12,13,479,497,66,421,54,44,423,406,430,63,64,336,625,58,354,355,0)) {}
      FIELD_SET(*this, FIX::Commission);
      FIELD_SET(*this, FIX::CommType);
      FIELD_SET(*this, FIX::CommCurrency);
      FIELD_SET(*this, FIX::FundRenewWaiv);
      FIELD_SET(*this, FIX::ListID);
      FIELD_SET(*this, FIX::Country);
      FIELD_SET(*this, FIX::Side);
      FIELD_SET(*this, FIX::Price);
      FIELD_SET(*this, FIX::PriceType);
      FIELD_SET(*this, FIX::FairValue);
      FIELD_SET(*this, FIX::NetGrossInd);
      FIELD_SET(*this, FIX::SettlType);
      FIELD_SET(*this, FIX::SettlDate);
      FIELD_SET(*this, FIX::TradingSessionID);
      FIELD_SET(*this, FIX::TradingSessionSubID);
      FIELD_SET(*this, FIX::Text);
      FIELD_SET(*this, FIX::EncodedTextLen);
      FIELD_SET(*this, FIX::EncodedText);
    };
  };

}

#endif
