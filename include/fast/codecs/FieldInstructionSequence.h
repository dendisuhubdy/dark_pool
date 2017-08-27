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
#ifndef FIELDINSTRUCTIONSEQUENCE_H
#define FIELDINSTRUCTIONSEQUENCE_H
#include <Codecs/FieldInstruction.h>
#include <Codecs/SegmentBody_fwd.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief Instruction to encode/decode a sequence.
    ///
    /// Sequences are constructed by capturing field definitions
    /// into a SegmentBody then adding that segment to the Sequence
    /// via the setSegmentBody() method.
    ///
    /// Sequences guide decoding by implementing decodeNop.  It uses
    /// the segment to decode fields into the currently active ValueMessageBuilder.
    ///
    /// An attempt to use any other instruction with a Sequence
    /// will lead to a TemplateDefinitionError exception being thrown.

    class QuickFAST_Export FieldInstructionSequence : public FieldInstruction
    {
    public:
      /// @brief construct with a name and a namespace
      /// @param name is the local name
      /// @param fieldNamespace is the namespace to qualify this name
      FieldInstructionSequence(
        const std::string & name,
        const std::string & fieldNamespace);

      /// @brief construct anonomous field instruction
      FieldInstructionSequence();

      /// @brief a typical virtual destructor.
      virtual ~FieldInstructionSequence();

      void setSegmentBody(Codecs::SegmentBodyPtr segment)
      {
        segment_ = segment;
      }

      virtual bool getSegmentBody(Codecs::SegmentBodyPtr & segment) const
      {
        segment = segment_;
        return bool(segment);
      }

      virtual void decodeNop(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void encodeNop(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void indexDictionaries(
        DictionaryIndexer & indexer,
        const std::string & dictionaryName,
        const std::string & typeName,
        const std::string & typeNamespace);

      virtual void finalize(TemplateRegistry & templateRegistry);

      virtual ValueType::Type fieldInstructionType()const;
      virtual void displayBody(std::ostream & output, size_t indent)const;

    private:
      void interpretValue(const std::string & value);
    private:
      Codecs::SegmentBodyPtr segment_;
    };
  }
}
#endif // FIELDINSTRUCTIONSEQUENCE_H
