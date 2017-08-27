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
#ifndef ENCODER_H
#define ENCODER_H
#include "Encoder_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Codecs/Context.h>
#include <Codecs/DataDestination_fwd.h>
#include <Codecs/PresenceMap_fwd.h>
#include <Codecs/Template.h>
#include <Codecs/SegmentBody_fwd.h>
#include <Messages/MessageAccessor.h>

#include <Common/Exceptions.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief Encode incoming FAST messages.
    ///
    /// Create an instance of the Encoder providing a registry of the templates
    /// to be used to encode the message, then call encodeMessage to encode
    /// each message from a DataDestination.
    class QuickFAST_Export Encoder : public Context
    {
    public:
      /// @brief Construct with a TemplateRegistry containing all templates to be used.
      /// @param registry A registry containing all templates to be used to encode messages.
      Encoder(Codecs::TemplateRegistryPtr registry);

      /// @brief Encode messages until the accessor is satisfied.
      ///
      /// MessageAccessor::pickTemplate() will be called to select a template.
      /// Each time it returns true (and a valid template ID) another message will be encoded.
      ///
      /// @param[out] destination where to write the encoded messages.
      /// @param[in] accessor to the fields to be encoded.
      void encodeMessages(
        DataDestination & destination,
        Messages::MessageAccessor & accessor);


      /// @brief Encode the next message using the specified template.
      /// @param[out] destination where to write the encoded messages.
      /// @param[in] templateId identifies the template to use for encoding.
      /// @param[in] accessor to the fields to be encoded.
      void encodeMessage(
        DataDestination & destination,
        template_id_t templateId,
        const Messages::MessageAccessor & accessor);

      /// @brief Encode a group field.
      ///
      /// @param[in] destination to which FAST data goes.
      /// @param[in] group defines the group [a subset of a template]
      /// @param[in] accessor to the fields to be encoded
      void
      encodeGroup(
        DataDestination & destination,
        const Codecs::SegmentBodyPtr & group,
        const Messages::MessageAccessor & accessor);

      /// @brief Encode a segment into a destination.
      ///
      /// @param[in] source supplies the FAST encoded data.
      /// @param[in] templateId identifies the template to be used during encoding
      /// @param[in] accessor to the fields to be encoded
      void encodeSegment(
        DataDestination & source,
        template_id_t templateId,
        const Messages::MessageAccessor & accessor);

      /// @brief Encode the body of a segment into a destination.
      ///
      /// @param[in] destination receives the FAST encoded data.
      /// @param[in] presenceMap is used to determine which fields are present
      ///        in the input.
      /// @param[in] segment defines the expected fields [part of a template]
      /// @param[in] accessor to which the encoded fields will be added
      void encodeSegmentBody(
        DataDestination & destination,
        Codecs::PresenceMap & presenceMap,
        const Codecs::SegmentBodyCPtr & segment,
        const Messages::MessageAccessor & accessor);
    private:
    };
  }
}
#endif // ENCODER_H
