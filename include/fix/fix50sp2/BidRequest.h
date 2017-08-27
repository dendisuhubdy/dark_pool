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

#ifndef FIX50SP2_BIDREQUEST_H
#define FIX50SP2_BIDREQUEST_H

#include "Message.h"

namespace FIX50SP2
{

  class BidRequest : public Message
  {
  public:
    BidRequest() : Message(MsgType()) {}
    BidRequest(const FIX::Message& m) : Message(m) {}
    BidRequest(const Message& m) : Message(m) {}
    BidRequest(const BidRequest& m) : Message(m) {}
    static FIX::MsgType MsgType() { return FIX::MsgType("k"); }

    BidRequest(
      const FIX::ClientBidID& aClientBidID,
      const FIX::BidRequestTransType& aBidRequestTransType,
      const FIX::TotNoRelatedSym& aTotNoRelatedSym,
      const FIX::BidType& aBidType,
      const FIX::BidTradeType& aBidTradeType,
      const FIX::BasisPxType& aBasisPxType )
    : Message(MsgType())
    {
      set(aClientBidID);
      set(aBidRequestTransType);
      set(aTotNoRelatedSym);
      set(aBidType);
      set(aBidTradeType);
      set(aBasisPxType);
    }

    FIELD_SET(*this, FIX::BidID);
    FIELD_SET(*this, FIX::ClientBidID);
    FIELD_SET(*this, FIX::BidRequestTransType);
    FIELD_SET(*this, FIX::ListName);
    FIELD_SET(*this, FIX::TotNoRelatedSym);
    FIELD_SET(*this, FIX::BidType);
    FIELD_SET(*this, FIX::NumTickets);
    FIELD_SET(*this, FIX::Currency);
    FIELD_SET(*this, FIX::SideValue1);
    FIELD_SET(*this, FIX::SideValue2);
    FIELD_SET(*this, FIX::NoBidDescriptors);
    class NoBidDescriptors: public FIX::Group
    {
    public:
    NoBidDescriptors() : FIX::Group(398,399,FIX::message_order(399,400,401,404,441,402,403,405,406,407,408,0)) {}
      FIELD_SET(*this, FIX::BidDescriptorType);
      FIELD_SET(*this, FIX::BidDescriptor);
      FIELD_SET(*this, FIX::SideValueInd);
      FIELD_SET(*this, FIX::LiquidityValue);
      FIELD_SET(*this, FIX::LiquidityNumSecurities);
      FIELD_SET(*this, FIX::LiquidityPctLow);
      FIELD_SET(*this, FIX::LiquidityPctHigh);
      FIELD_SET(*this, FIX::EFPTrackingError);
      FIELD_SET(*this, FIX::FairValue);
      FIELD_SET(*this, FIX::OutsideIndexPct);
      FIELD_SET(*this, FIX::ValueOfFutures);
    };
    FIELD_SET(*this, FIX::NoBidComponents);
    class NoBidComponents: public FIX::Group
    {
    public:
    NoBidComponents() : FIX::Group(420,66,FIX::message_order(66,54,336,625,430,63,64,1,660,0)) {}
      FIELD_SET(*this, FIX::ListID);
      FIELD_SET(*this, FIX::Side);
      FIELD_SET(*this, FIX::TradingSessionID);
      FIELD_SET(*this, FIX::TradingSessionSubID);
      FIELD_SET(*this, FIX::NetGrossInd);
      FIELD_SET(*this, FIX::SettlType);
      FIELD_SET(*this, FIX::SettlDate);
      FIELD_SET(*this, FIX::Account);
      FIELD_SET(*this, FIX::AcctIDSource);
    };
    FIELD_SET(*this, FIX::LiquidityIndType);
    FIELD_SET(*this, FIX::WtAverageLiquidity);
    FIELD_SET(*this, FIX::ExchangeForPhysical);
    FIELD_SET(*this, FIX::OutMainCntryUIndex);
    FIELD_SET(*this, FIX::CrossPercent);
    FIELD_SET(*this, FIX::ProgRptReqs);
    FIELD_SET(*this, FIX::ProgPeriodInterval);
    FIELD_SET(*this, FIX::IncTaxInd);
    FIELD_SET(*this, FIX::ForexReq);
    FIELD_SET(*this, FIX::NumBidders);
    FIELD_SET(*this, FIX::TradeDate);
    FIELD_SET(*this, FIX::BidTradeType);
    FIELD_SET(*this, FIX::BasisPxType);
    FIELD_SET(*this, FIX::StrikeTime);
    FIELD_SET(*this, FIX::Text);
    FIELD_SET(*this, FIX::EncodedTextLen);
    FIELD_SET(*this, FIX::EncodedText);
  };

}

#endif
