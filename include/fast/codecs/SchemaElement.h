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

#ifndef SCHEMAELEMENT_H
#define SCHEMAELEMENT_H
#include "SchemaElement_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Codecs/Template_fwd.h>
#include <Codecs/TemplateRegistry_fwd.h>
#include <Codecs/FieldInstruction_fwd.h>
#include <Codecs/FieldOp_fwd.h>
#include <Codecs/SegmentBody_fwd.h>

namespace QuickFAST
{
  namespace Codecs
  {
    /// @brief Base class for elements found in an XML template file
    ///
    /// The virtual methods in this class represent actions which an
    /// element needs to apply to its parent/container while building
    /// a template repository.
    ///
    /// Because not all actions are available on all containers/parents
    /// the implementation of these methods here simply throws a TemplateDefinition
    /// exception.  Classes for which these actions do make sense will override
    /// the methods.
    class QuickFAST_Export SchemaElement
    {
    public:
      /// @brief Destructor.
      virtual ~SchemaElement()
      {
      }

      /// @brief After all contained elements have been added, do any necessary housekeeping.
      virtual void finalize(TemplateRegistry & templateRegistry);

      /// @brief Add a template to a container
      /// @param value smart pointer to the template to be added
      virtual void addTemplate(TemplatePtr value);

      /// @brief Set a field operation
      ///
      /// Assigns the appropriate dispatching object to a fieldInstruction.
      virtual void setFieldOp(FieldOpPtr fieldOp);

      /// @brief Implement the TypeRef element.
      ///
      /// Defines an application type corresponding to this segment.
      /// The spec does not assign any semantics to "application type."
      /// For QuickFAST, a Group that contains a &lt;typeref> element
      /// maintains it's identity in the decoded Message.  One that
      /// does not is simply merged into to containing FileSet as it
      /// is decoded.
      /// @param type the name= attribute of the typeref element
      /// @param ns is the namespace for the type.
      virtual void setApplicationType(const std::string & type, const std::string & ns);

      /// @brief Implement &lt;length> within a &lt;sequence> or &lt;byteVector>
      ///
      /// @param field points to the field to be added.  The smart pointer to this
      /// field will be kept in the segment definition.
      virtual void addLengthInstruction(FieldInstructionPtr & field);

      /// @brief Add a new field definition to the segment
      ///
      /// @param field points to the field to be added.  The smart pointer to this
      /// field will be kept in the segment definition.
      virtual void addInstruction(FieldInstructionPtr & field);

      /// @brief Support &lt;mantissa> element.
      ///
      /// The mantissa field instruction may have special operations or properties.
      virtual void setMantissaInstruction(FieldInstructionPtr mantissa);

      /// @brief Get the mantissa field instruction.
      virtual bool getMantissaInstruction(FieldInstructionCPtr & mantissa) const;

      /// @brief Support &lt;exponent> element.
      ///
      /// The exponent field instruction may have special operations or properties.
      virtual void setExponentInstruction(FieldInstructionPtr exponent);

      /// @brief Get the exponent field instruction.
      virtual bool getExponentInstruction(FieldInstructionCPtr & exponent) const;
    };
  }
}

#endif /* SCHEMAELEMENT_H */
