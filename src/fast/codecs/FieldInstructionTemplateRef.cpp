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
#include "FieldInstructionTemplateRef.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/Encoder.h>
#include <Codecs/TemplateRegistry.h>
#include <Messages/ValueMessageBuilder.h>
#include <Messages/Group.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstructionStaticTemplateRef::FieldInstructionStaticTemplateRef(
  const std::string & name, const std::string & fieldNamespace)
  : FieldInstruction(name, fieldNamespace)
  , templateName_(name)
  , templateNamespace_(fieldNamespace)
  , isFinalized_(false)
  , fieldCount_(0)
{
}

FieldInstructionStaticTemplateRef::FieldInstructionStaticTemplateRef()
  : isFinalized_(false)
  , fieldCount_(0)
{
}

FieldInstructionStaticTemplateRef::~FieldInstructionStaticTemplateRef()
{
}


bool
FieldInstructionStaticTemplateRef::isPossiblyRecursive() const
{
  return true;
}

void
FieldInstructionStaticTemplateRef::finalize(TemplateRegistry & templateRegistry)
{
  TemplatePtr target;
  if(!templateRegistry.findNamedTemplate(templateName_, templateNamespace_, target))
  {
    std::stringstream exception;
    exception << "[ERR D9] Unknown template name for static templateref." << identity_->name();
    throw QuickFAST::TemplateDefinitionError(exception.str());
  }
  target->finalize(templateRegistry);
  // subtract one for the template ID
  presenceMapBitsUsed_ = target->presenceMapBitCount() - 1;
  fieldCount_ = target->fieldCount();
  isFinalized_ = true;
}

void
FieldInstructionStaticTemplateRef::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & messageBuilder) const
{
  TemplateCPtr target;
  if(!decoder.findTemplate(templateName_, templateNamespace_, target))
  {
    decoder.reportFatal("[ERR D9]", "Unknown template name for static templateref.", *identity_);
  }

  if(messageBuilder.getApplicationType() != target->getApplicationType())
  {
    Messages::ValueMessageBuilder & groupBuilder(
      messageBuilder.startGroup(
        identity_,
        target->getApplicationType(),
        target->getApplicationTypeNamespace(),
        target->fieldCount()));

    decoder.decodeSegmentBody(source, pmap, target, groupBuilder);
    messageBuilder.endGroup(
      identity_,
      groupBuilder);

  }
  else
  {
    // Because the application types match,
    // the templateRef fields are decoded directly into to the current
    // field set.  As a result the templateRef "disappears" completely
    // from the application message.  This is a good thing.
    // The same message encoded with different
    // templates could be transmitted with different sets of fields defined
    // by templateRefs, but the underlying application type should not reflect
    // the technique used to encode/decode it.
    decoder.decodeSegmentBody(source, pmap, target, messageBuilder);
  }
}

void
FieldInstructionStaticTemplateRef::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  // static templateRef
  // static
  TemplateCPtr target;
  if(!encoder.findTemplate(templateName_, templateNamespace_, target))
  {
    encoder.reportFatal("[ERR D9]", "Unknown template name for static templateref.", *identity_);
  }

  // retrieve the field corresponding to this templateRef
  // which if it exists should be a FieldGroup
  const QuickFAST::Messages::MessageAccessor * group;
  if(accessor.getGroup(*identity_, group))
  {
    encoder.encodeSegmentBody(
      destination,
      pmap,
      target,
      *group);
  }
  else
  {
    encoder.encodeSegmentBody(
      destination,
      pmap,
      target,
      accessor);
  }
}

void
FieldInstructionStaticTemplateRef::interpretValue(const std::string & value)
{
  throw TemplateDefinitionError("Unexpected field operation for templateref");
}

size_t
FieldInstructionStaticTemplateRef::fieldCount(const SegmentBody & parent)const
{
  if(!isFinalized_)
  {
    throw TemplateDefinitionError("Field count requested from static template reference before it has been finalized.");
  }
  return fieldCount_;
}

ValueType::Type
FieldInstructionStaticTemplateRef::fieldInstructionType()const
{
  return ValueType::TEMPLATEREF;
}


/////////////////////////////////////
// FieldInstructionDynamicTemplateRef
FieldInstructionDynamicTemplateRef::FieldInstructionDynamicTemplateRef()
  : FieldInstruction("dynamic template reference", "")
{
}

FieldInstructionDynamicTemplateRef::~FieldInstructionDynamicTemplateRef()
{
}

void
FieldInstructionDynamicTemplateRef::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & messageBuilder) const
{
   decoder.decodeNestedTemplate(source, messageBuilder, this->getIdentity());
}

void
FieldInstructionDynamicTemplateRef::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & accessor) const
{
  encoder.reportFatal("[ERR I1]", "Encoding dynamic templates is not supported.");
}

void
FieldInstructionDynamicTemplateRef::interpretValue(const std::string & value)
{
  throw TemplateDefinitionError("Unexpected field operation for templateref");
}

size_t
FieldInstructionDynamicTemplateRef::fieldCount(const SegmentBody & parent)const
{
  // TODO: Someday we should actually check the target template, but that's
  // not doable right now.
  return 1;
}

ValueType::Type
FieldInstructionDynamicTemplateRef::fieldInstructionType()const
{
  return ValueType::TEMPLATEREF;
}
