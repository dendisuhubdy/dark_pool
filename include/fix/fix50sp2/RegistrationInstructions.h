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

#ifndef FIX50SP2_REGISTRATIONINSTRUCTIONS_H
#define FIX50SP2_REGISTRATIONINSTRUCTIONS_H

#include "Message.h"

namespace FIX50SP2
{

  class RegistrationInstructions : public Message
  {
  public:
    RegistrationInstructions() : Message(MsgType()) {}
    RegistrationInstructions(const FIX::Message& m) : Message(m) {}
    RegistrationInstructions(const Message& m) : Message(m) {}
    RegistrationInstructions(const RegistrationInstructions& m) : Message(m) {}
    static FIX::MsgType MsgType() { return FIX::MsgType("o"); }

    RegistrationInstructions(
      const FIX::RegistID& aRegistID,
      const FIX::RegistTransType& aRegistTransType,
      const FIX::RegistRefID& aRegistRefID )
    : Message(MsgType())
    {
      set(aRegistID);
      set(aRegistTransType);
      set(aRegistRefID);
    }

    FIELD_SET(*this, FIX::RegistID);
    FIELD_SET(*this, FIX::RegistTransType);
    FIELD_SET(*this, FIX::RegistRefID);
    FIELD_SET(*this, FIX::ClOrdID);
    FIELD_SET(*this, FIX::NoPartyIDs);
    class NoPartyIDs: public FIX::Group
    {
    public:
    NoPartyIDs() : FIX::Group(453,448,FIX::message_order(448,447,452,802,0)) {}
      FIELD_SET(*this, FIX::PartyID);
      FIELD_SET(*this, FIX::PartyIDSource);
      FIELD_SET(*this, FIX::PartyRole);
      FIELD_SET(*this, FIX::NoPartySubIDs);
      class NoPartySubIDs: public FIX::Group
      {
      public:
      NoPartySubIDs() : FIX::Group(802,523,FIX::message_order(523,803,0)) {}
        FIELD_SET(*this, FIX::PartySubID);
        FIELD_SET(*this, FIX::PartySubIDType);
      };
    };
    FIELD_SET(*this, FIX::Account);
    FIELD_SET(*this, FIX::AcctIDSource);
    FIELD_SET(*this, FIX::RegistAcctType);
    FIELD_SET(*this, FIX::TaxAdvantageType);
    FIELD_SET(*this, FIX::OwnershipType);
    FIELD_SET(*this, FIX::NoRegistDtls);
    class NoRegistDtls: public FIX::Group
    {
    public:
    NoRegistDtls() : FIX::Group(473,509,FIX::message_order(509,511,474,482,539,522,486,475,0)) {}
      FIELD_SET(*this, FIX::RegistDtls);
      FIELD_SET(*this, FIX::RegistEmail);
      FIELD_SET(*this, FIX::MailingDtls);
      FIELD_SET(*this, FIX::MailingInst);
      FIELD_SET(*this, FIX::NoNestedPartyIDs);
      class NoNestedPartyIDs: public FIX::Group
      {
      public:
      NoNestedPartyIDs() : FIX::Group(539,524,FIX::message_order(524,525,538,804,0)) {}
        FIELD_SET(*this, FIX::NestedPartyID);
        FIELD_SET(*this, FIX::NestedPartyIDSource);
        FIELD_SET(*this, FIX::NestedPartyRole);
        FIELD_SET(*this, FIX::NoNestedPartySubIDs);
        class NoNestedPartySubIDs: public FIX::Group
        {
        public:
        NoNestedPartySubIDs() : FIX::Group(804,545,FIX::message_order(545,805,0)) {}
          FIELD_SET(*this, FIX::NestedPartySubID);
          FIELD_SET(*this, FIX::NestedPartySubIDType);
        };
      };
      FIELD_SET(*this, FIX::OwnerType);
      FIELD_SET(*this, FIX::DateOfBirth);
      FIELD_SET(*this, FIX::InvestorCountryOfResidence);
    };
    FIELD_SET(*this, FIX::NoDistribInsts);
    class NoDistribInsts: public FIX::Group
    {
    public:
    NoDistribInsts() : FIX::Group(510,477,FIX::message_order(477,512,478,498,499,500,501,502,0)) {}
      FIELD_SET(*this, FIX::DistribPaymentMethod);
      FIELD_SET(*this, FIX::DistribPercentage);
      FIELD_SET(*this, FIX::CashDistribCurr);
      FIELD_SET(*this, FIX::CashDistribAgentName);
      FIELD_SET(*this, FIX::CashDistribAgentCode);
      FIELD_SET(*this, FIX::CashDistribAgentAcctNumber);
      FIELD_SET(*this, FIX::CashDistribPayRef);
      FIELD_SET(*this, FIX::CashDistribAgentAcctName);
    };
  };

}

#endif
