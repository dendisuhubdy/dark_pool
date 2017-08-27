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

#ifndef FIX_VALUES_BASE_H
#define FIX_VALUES_BASE_H

#include "FixValues.h"
#include <string>

namespace FIX
{
  const char BeginString_FIXT11[] = "FIXT.1.1";
  const char BeginString_FIX50[] = "FIX.5.0";
  const char BeginString_FIX44[] = "FIX.4.4";
  const char BeginString_FIX43[] = "FIX.4.3";
  const char BeginString_FIX42[] = "FIX.4.2";
  const char BeginString_FIX41[] = "FIX.4.1";
  const char BeginString_FIX40[] = "FIX.4.0";

  const char SessionRejectReason_INVALID_TAG_NUMBER_TEXT[] = "Invalid tag number";
  const char SessionRejectReason_REQUIRED_TAG_MISSING_TEXT[] = "Required tag missing";
  const char SessionRejectReason_TAG_NOT_DEFINED_FOR_THIS_MESSAGE_TYPE_TEXT[] = "Tag not defined for this message type";
  const char SessionRejectReason_UNDEFINED_TAG_TEXT[] = "Undefined Tag";
  const char SessionRejectReason_TAG_SPECIFIED_WITHOUT_A_VALUE_TEXT[] = "Tag specified without a value";
  const char SessionRejectReason_VALUE_IS_INCORRECT_TEXT[] = "Value is incorrect (out of range) for this tag";
  const char SessionRejectReason_INCORRECT_DATA_FORMAT_FOR_VALUE_TEXT[] = "Incorrect data format for value";
  const char SessionRejectReason_DECRYPTION_PROBLEM_TEXT[] = "Decryption problem";
  const char SessionRejectReason_SIGNATURE_PROBLEM_TEXT[] = "Signature problem";
  const char SessionRejectReason_COMPID_PROBLEM_TEXT[] = "CompID problem";
  const char SessionRejectReason_SENDINGTIME_ACCURACY_PROBLEM_TEXT[] = "SendingTime accuracy problem";
  const char SessionRejectReason_INVALID_MSGTYPE_TEXT[] = "Invalid MsgType";
  const char SessionRejectReason_TAG_APPEARS_MORE_THAN_ONCE_TEXT[] = "Tag appears more than once";
  const char SessionRejectReason_TAG_SPECIFIED_OUT_OF_REQUIRED_ORDER_TEXT[] = "Tag specified out of required order";
  const char SessionRejectReason_INCORRECT_NUMINGROUP_COUNT_FOR_REPEATING_GROUP_TEXT[] = "Incorrect NumInGroup count for repeating group";
  const char BusinessRejectReason_OTHER_TEXT[] = "Other";
  const char BusinessRejectReason_UNKNOWN_ID_TEXT[] = "Unknown ID";
  const char BusinessRejectReason_UNKNOWN_SECURITY_TEXT[] = "Unknown Security";
  const char BusinessRejectReason_UNSUPPORTED_MESSAGE_TYPE_TEXT[] = "Unsupported Message Type";
  const char BusinessRejectReason_APPLICATION_NOT_AVAILABLE_TEXT[] = "Application Not Available";
  const char BusinessRejectReason_CONDITIONALLY_REQUIRED_FIELD_MISSING_TEXT[] = "Conditionally Required Field Missing";
  const char BusinessRejectReason_NOT_AUTHORIZED_TEXT[] = "Not Authorized";
  const char BusinessRejectReason_DELIVERTO_FIRM_NOT_AVAILABLE_AT_THIS_TIME_TEXT[] = "Deliver to firm not available at this time";
}
#endif //FIX_VALUES_BASE_H
