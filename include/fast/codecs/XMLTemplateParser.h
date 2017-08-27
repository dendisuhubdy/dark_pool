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

#ifndef XMLTEMPLATEPARSER_H
#define XMLTEMPLATEPARSER_H

#include "XMLTemplateParser_fwd.h"

#include <Codecs/TemplateRegistry_fwd.h>
#include <Codecs/SchemaElement_fwd.h>
#include <Common/QuickFAST_Export.h>
#include <istream>

namespace QuickFAST
{
  namespace Codecs
  {
    /// @brief Parse a template file into a TemplateRegistry.
    ///
    /// Notes on parsing XML Templates
    ///
    /// The goal is to populate a TemplateRegistry with a set of
    /// Template objects.  A Template is a SegmentBody.
    /// A SegmentBody is an ordered collection of FieldInstructions.
    ///
    /// The intent is to have almost every element in the XML template
    /// file correspond to an object, and have the attributes for those
    /// elements correspond to method calls on those objects.
    /// In practice, some elements correspond to method calls on the
    /// enclosing element.
    ///
    /// The mapping:<pre>
    /// XML element                 Object or method
    /// ========================    ================
    /// &lt;templates>                 TemplateRegistry
    /// &lt;template>                  Template
    ///                             -- add to registry via TemplateRegistry::addTemplate
    /// &lt;typeRef>                   setApplicationType() on enclosing element.
    ///
    /// Field instructions          add to Template via addInstruction() method.
    ///   &lt;int32>                   FieldInstructionInt32
    ///   &lt;int64>                   FieldInstructionInt64
    ///   &lt;uint32>                  FieldInstructionUInt32
    ///   &lt;uint64>                  FieldInstructionUInt64
    ///   &lt;decimal>                 FieldInstructionDecimal
    ///    &lt;exponent>               FieldInstructionExponent
    ///                               add via setExponentInstruction()
    ///    &lt;mantissa>               FieldInstructionMantissa
    ///                               add via setMantissaInstruction()
    ///   &lt;string charset="ascii">  FieldInstructionAscii [note charset is optional here]
    ///   &lt;string charset="unicode"> FieldInstructionUtf8
    ///   &lt;byteVector>              FieldInstructionByteVector
    ///   &lt;group>                   FieldInstructionGroup (starts new Segment)
    ///   &lt;sequence>                FieldInstructionSequence (starts new Segment)
    ///     &lt;length>                FieldInstructionUInt32 added to sequence via
    ///                               addLengthInstruction() method.
    ///   &lt;templateRef>             FieldInstructionTemplateRef
    ///
    /// Field operations            add to FieldInstruction via setFieldOp() method
    ///     value=                    setValue() method on FieldOp
    ///     key=                      setKey() method on FieldOp
    ///     nsKey=                    setKeyNamespace() method on FieldOp
    ///     dictionary=               setDictionaryName() method on FieldOp
    ///   no operation specified    FieldOpNop (set by default)
    ///   &lt;constant>                FieldOpConstant
    ///   &lt;default>                 FieldOpDefault
    ///   &lt;copy>                    FieldOpCopy
    ///   &lt;delta>                   FieldOpDelta
    ///   &lt;increment>               FieldOpIncrement
    ///   &lt;tail>                    FieldOpTail
    ///
    /// Commmon attributes
    ///   name=                     construction parameter or setName()
    ///   ns=                       construction parameter or setNamespace()
    ///   id=                       setId()
    ///   presence="mandatory"      setPresence(true)  (the default)
    ///   presence="optional"       setPresence(false)
    /// </pre>
    /// Notes:
    ///
    /// &lt;typeRef>
    ///   The typeref instruction should really be an attribute on the
    ///   Template, FieldInstructionGroup, or FieldInstructionSequence.
    ///   Thus rather than defining FieldInstructionTypeRef, there is
    ///   a method: setApplicationType() on the SegmentBody object, and a
    ///   typeref element should trigger a call to that method.  Note that
    ///   only one typeref is allowed per segment.
    ///
    ///
    /// &lt;templateRef>
    ///   A FieldInstructionGroup contains a SegmentBody for the fields that are
    ///   defined within the group.
    ///   When the start element of a Group or Sequence is encountered, a new
    ///   Codecs::SegmentBody should be created.  Parsing should continue into
    ///   that SegmentBody until the end of the Group.
    ///
    /// &lt;sequence>
    ///   A FieldInstructionSequence contains a SegmentBody for the fields that are
    ///   defined within the sequence.
    ///   When the start element of a Sequence is encountered, a new Codecs::SegmentBody
    ///   should be created.
    ///
    ///   If a &lt;length> instruction is encountered while decoding a segment, a
    ///   FieldInstructionUInt32 should be created, honoring any attributes in the
    ///   instruction element.   This FieldInstruction should be added to the segment
    ///   using setLengthInstruction rather than the normal addInstruction method.
    ///
    ///   The segment will detect illegal uses of the &lt;length> instruction.  If
    ///   No length instruction is specified, the Sequence will automatically handle
    ///   the default length.
    ///
    /// Multithreading Note:
    ///   This class was designed under the assumption that it would be run once during
    ///   initialization.  There are no guarantees that template parsing is thread-safe.
    ///   The resulting template repository, however, can be safely shared between
    ///   threads.

    class QuickFAST_Export XMLTemplateParser
    {
    public:
      XMLTemplateParser();
      ~XMLTemplateParser();

      /// @brief Parse XML data from an input stream
      /// @param xmlData is the stream that supplies the data
      /// @returns a TemplateRegistry containing the parsed templates.
      TemplateRegistryPtr
      parse(
        std::istream& xmlData
        );

      /// @brief enable verbosity and give it a destination
      /// @param out the stream to which diagnostic info should be written
      void setVerboseOutput(std::ostream & out);

      /// @brief Assign bits to non-standard features.
      enum NonstandardFeatures
      {
        NONSTANDARD_PresenceOnLengthInstruction = 1
      };

      /// @brief Enable nonstandard behavior.
      ///
      /// Some non-standard implementatons of FAST do not comply with the Specification
      /// @param nonstandard is a bitwise OR of the nonstandard featurse that should be allowed
      void setNonstandard(unsigned long nonstandard);
    private:
      // forbid copy constructor
      XMLTemplateParser(const XMLTemplateParser &);
      // forbid assignment
      XMLTemplateParser & operator = (const XMLTemplateParser &);
    private:
      std::ostream * out_;
      unsigned long nonstandard_;
    };
  }
}

#endif /* XMLTEMPLATEPARSER_H */
