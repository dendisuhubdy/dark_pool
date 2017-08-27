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
#ifndef FIELDINSTRUCTIONTEMPLATEREF_H
#define FIELDINSTRUCTIONTEMPLATEREF_H
#include <Codecs/FieldInstruction.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief Implement static &lt;templateRef> field instruction.
    class QuickFAST_Export FieldInstructionStaticTemplateRef : public FieldInstruction
    {
    public:
      /// @brief construct with a name and a namespace
      /// @param name is the local name
      /// @param fieldNamespace is the namespace to qualify this name
      FieldInstructionStaticTemplateRef(
        const std::string & name,
        const std::string & fieldNamespace);

      /// @brief construct anonomous field instruction
      FieldInstructionStaticTemplateRef();

      /// @brief a typical virtual destructor.
      virtual ~FieldInstructionStaticTemplateRef();

      ///////////////////////////////////////
      /// Implement FieldInstruction methods
      virtual void finalize(TemplateRegistry & templateRegistry);
      virtual bool isPossiblyRecursive() const;
      virtual size_t fieldCount(const SegmentBody & parent)const;
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
      virtual ValueType::Type fieldInstructionType()const;

    private:
      void interpretValue(const std::string & value);

    private:
      std::string templateName_;
      std::string templateNamespace_;
      bool isFinalized_;
      size_t fieldCount_; // how many fields are in the target template (valid after finalize has been called)
    };

    /// @brief Implement dynamic &lt;templateRef> field instruction.
    class QuickFAST_Export FieldInstructionDynamicTemplateRef : public FieldInstruction
    {
    public:
      /// @brief construct anonomous field instruction
      FieldInstructionDynamicTemplateRef();

      /// @brief a typical virtual destructor.
      virtual ~FieldInstructionDynamicTemplateRef();

      virtual size_t fieldCount(const SegmentBody & parent)const;

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

      virtual ValueType::Type fieldInstructionType()const;

    private:
      void interpretValue(const std::string & value);

    private:
    };

  }
}
#endif // FIELDINSTRUCTIONTEMPLATEREF_H
