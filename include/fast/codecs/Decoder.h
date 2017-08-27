// Copyright (c) 2009, 2010, 2011 Object Computing, Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright notice,
//      this list of conditions and the following disclaimer in the documentation
//      and/or other materials provided with the distribution.
//    * Neither the name of Object Computing, Inc. nor the names of its
//      contributors may be used to endorse or promote products derived from this
//      software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifdef _MSC_VER
# pragma once
#endif
#ifndef DECODER_H
#define DECODER_H
#include "Decoder_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Codecs/Context.h>
#include <Codecs/DataSource_fwd.h>
#include <Codecs/PresenceMap_fwd.h>
#include <Codecs/Template.h>
#include <Codecs/SegmentBody_fwd.h>
#include <Messages/ValueMessageBuilder_fwd.h>

#include <Common/Exceptions.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief Decode incoming FAST messages.
    ///
    /// Create an instance of the Decoder providing a registry of the templates
    /// to be used to decode the message, then call decodeMessage to decode
    /// each message from a DataSource.
    class QuickFAST_Export Decoder : public Context
    {
    public:
      /// @brief Construct with a TemplateRegistry containing all templates to be used.
      /// @param registry A registry containing all templates to be used to decode messages.
      explicit Decoder(TemplateRegistryPtr registry);

      /// @brief Decode the next message.
      /// @param[in] source where to read the incoming message(s).
      /// @param[out] message an empty message into which the decoded fields will be stored.
      void decodeMessage(
        DataSource & source,
        Messages::ValueMessageBuilder & message);

      /// @brief Decode a group field.
      ///
      /// If the application type of the group matches the application type of the
      /// messageBuilder parameter then fields in the group will be added to the messageBuilder
      /// If they differ then a GroupField will be created and added to the messageBuilder,
      /// and the fields in the group will be added to the messageBuilder contained in the
      /// new GroupField.
      ///
      /// @param[in] source from which the FAST data comes
      /// @param[in] segment defines the group [a subset of a template]
      /// @param[in] messageBuilder to which the decoded fields will be added
      void
      decodeGroup(
        DataSource & source,
        const SegmentBodyCPtr & segment,
        Messages::ValueMessageBuilder & messageBuilder);

      /// @brief Decode a segment into a messageBuilder.
      ///
      /// @param[in] source supplies the FAST encoded data.
      /// @param[in] messageBuilder to which the decoded fields will be added
      /// @param[in] identity names the template.
      void decodeNestedTemplate(
        DataSource & source,
        Messages::ValueMessageBuilder & messageBuilder,
        Messages::FieldIdentityCPtr & identity);

      /// @brief Decode the body of a segment into a messageBuilder.
      ///
      /// @param[in] source supplies the FAST encoded data.
      /// @param[in] pmap is used to determine which fields are present
      ///        in the input.
      /// @param[in] segment defines the expected fields [part of a template]
      /// @param[in] messageBuilder to which the decoded fields will be added
      void decodeSegmentBody(
        DataSource & source,
        PresenceMap & pmap,
        const SegmentBodyCPtr & segment,
        Messages::ValueMessageBuilder & messageBuilder);
    };
  }
}
#endif // DECODER_H
