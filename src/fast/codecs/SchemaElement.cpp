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

#include <Common/QuickFASTPch.h>
#include "SchemaElement.h"
#include <Common/Exceptions.h>
#include <Codecs/FieldOp.h>
using namespace QuickFAST;
using namespace QuickFAST::Codecs;

void
SchemaElement::finalize(TemplateRegistry & templateRegistry)
{
  // nothing to do by default
}

void
SchemaElement::addTemplate(TemplatePtr value)
{
  throw TemplateDefinitionError("<template> not allowed in this context.");
}

void
SchemaElement::setApplicationType(const std::string & /*type*/, const std::string & /*ns*/)
{
  throw TemplateDefinitionError("<typeRef> not allowed in this context.");
}
void
SchemaElement::setFieldOp(FieldOpPtr fieldOp)
{
  std::stringstream msg;
  msg << "Field operation <" << fieldOp->opName(fieldOp->opType()) << "> not allowed in this context.";
  throw TemplateDefinitionError(msg.str());
}
void
SchemaElement::addLengthInstruction(FieldInstructionPtr & /*field*/)
{
  throw TemplateDefinitionError("<length> not allowed in this context.");
}


void
SchemaElement::addInstruction(FieldInstructionPtr & /*field*/)
{
  throw TemplateDefinitionError("Field instruction not allowed in this context.");
}
void
SchemaElement::setMantissaInstruction(FieldInstructionPtr mantissa)
{
  throw TemplateDefinitionError("Mantissa applies only to Decimal Field instruction.");
}

bool
SchemaElement::getMantissaInstruction(FieldInstructionCPtr & /*mantissa*/) const
{
  throw TemplateDefinitionError("Mantissa applies only to Decimal Field instruction.");
}

void
SchemaElement::setExponentInstruction(FieldInstructionPtr exponent)
{
  throw TemplateDefinitionError("Exponent applies only to Decimal Field instruction.");
}

bool
SchemaElement::getExponentInstruction(FieldInstructionCPtr & /*exponent*/) const
{
  throw TemplateDefinitionError("Exponent applies only to Decimal Field instruction.");
}
