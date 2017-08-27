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

#ifndef FIX50SP2_MARKETDEFINITION_H
#define FIX50SP2_MARKETDEFINITION_H

#include "Message.h"

namespace FIX50SP2
{

  class MarketDefinition : public Message
  {
  public:
    MarketDefinition() : Message(MsgType()) {}
    MarketDefinition(const FIX::Message& m) : Message(m) {}
    MarketDefinition(const Message& m) : Message(m) {}
    MarketDefinition(const MarketDefinition& m) : Message(m) {}
    static FIX::MsgType MsgType() { return FIX::MsgType("BU"); }

    MarketDefinition(
      const FIX::MarketReportID& aMarketReportID,
      const FIX::MarketID& aMarketID )
    : Message(MsgType())
    {
      set(aMarketReportID);
      set(aMarketID);
    }

    FIELD_SET(*this, FIX::MarketReportID);
    FIELD_SET(*this, FIX::MarketReqID);
    FIELD_SET(*this, FIX::MarketID);
    FIELD_SET(*this, FIX::MarketSegmentID);
    FIELD_SET(*this, FIX::MarketSegmentDesc);
    FIELD_SET(*this, FIX::EncodedMktSegmDescLen);
    FIELD_SET(*this, FIX::EncodedMktSegmDesc);
    FIELD_SET(*this, FIX::ParentMktSegmID);
    FIELD_SET(*this, FIX::Currency);
    FIELD_SET(*this, FIX::ExpirationCycle);
    FIELD_SET(*this, FIX::MinTradeVol);
    FIELD_SET(*this, FIX::MaxTradeVol);
    FIELD_SET(*this, FIX::MaxPriceVariation);
    FIELD_SET(*this, FIX::ImpliedMarketIndicator);
    FIELD_SET(*this, FIX::TradingCurrency);
    FIELD_SET(*this, FIX::RoundLot);
    FIELD_SET(*this, FIX::MultilegModel);
    FIELD_SET(*this, FIX::MultilegPriceMethod);
    FIELD_SET(*this, FIX::PriceType);
    FIELD_SET(*this, FIX::NoOrdTypeRules);
    class NoOrdTypeRules: public FIX::Group
    {
    public:
    NoOrdTypeRules() : FIX::Group(1237,40,FIX::message_order(40,0)) {}
      FIELD_SET(*this, FIX::OrdType);
    };
    FIELD_SET(*this, FIX::NoTimeInForceRules);
    class NoTimeInForceRules: public FIX::Group
    {
    public:
    NoTimeInForceRules() : FIX::Group(1239,59,FIX::message_order(59,0)) {}
      FIELD_SET(*this, FIX::TimeInForce);
    };
    FIELD_SET(*this, FIX::NoExecInstRules);
    class NoExecInstRules: public FIX::Group
    {
    public:
    NoExecInstRules() : FIX::Group(1232,1308,FIX::message_order(1308,0)) {}
      FIELD_SET(*this, FIX::ExecInstValue);
    };
    FIELD_SET(*this, FIX::TransactTime);
    FIELD_SET(*this, FIX::Text);
    FIELD_SET(*this, FIX::EncodedTextLen);
    FIELD_SET(*this, FIX::EncodedText);
    FIELD_SET(*this, FIX::ApplID);
    FIELD_SET(*this, FIX::ApplSeqNum);
    FIELD_SET(*this, FIX::ApplLastSeqNum);
    FIELD_SET(*this, FIX::ApplResendFlag);
  };

}

#endif
