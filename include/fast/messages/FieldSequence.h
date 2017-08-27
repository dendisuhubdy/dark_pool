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
#ifndef FIELDSEQUENCE_H
#define FIELDSEQUENCE_H
#include <Messages/Field.h>
#include <Messages/FieldSet_fwd.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing an ordered sequence of groups
    ///
    /// In the XML template file this field is described as &lt;sequence>
    class QuickFAST_Export FieldSequence : public Field{
      /// @brief Construct a field given a sequence for it to contain
      FieldSequence(Messages::SequenceCPtr sequence);
      /// @brief a typical virtual destructor.
      virtual ~FieldSequence();
    public:
      /// @brief Construct a field given a sequence for it to contain
      /// @param sequence the entries for this FieldSequence
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(Messages::SequenceCPtr sequence);
      /// @brief Construct a NULL field (not an empty string)
      /// @returns a constant pointer to the immutable field
      static FieldCPtr createNull();

      /// @brief Set the sequence directly(for debugging)
//      void setSequence(Messages::SequenceCPtr sequence);

      // implement selected virtual methods from Field
      virtual uint32 toUInt32()const;
      virtual const Messages::SequenceCPtr & toSequence() const;
      virtual bool operator == (const Field & rhs)const;
      virtual void valueToStringBuffer()const;
    private:
      Messages::SequenceCPtr sequence_;
    };
  }
}
#endif // FIELDINT64_H
