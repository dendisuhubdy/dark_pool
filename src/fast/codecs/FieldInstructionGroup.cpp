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
#include "FieldInstructionGroup.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/Encoder.h>
#include <Messages/Group.h>
#include <Messages/ValueMessageBuilder.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstructionGroup::FieldInstructionGroup(
  const std::string & name,
  const std::string & fieldNamespace)
  : FieldInstruction(name, fieldNamespace)
{
}

FieldInstructionGroup::FieldInstructionGroup()
{
}

FieldInstructionGroup::~FieldInstructionGroup()
{
}

void
FieldInstructionGroup::finalize(TemplateRegistry & templateRegistry)
{
  segmentBody_->finalize(templateRegistry);
  FieldInstruction::finalize(templateRegistry);
  // even though the field op is a NOP, an optional group uses a pmap bit
  if(!isMandatory())
  {
    presenceMapBitsUsed_ = 1;
  }
}

void
FieldInstructionGroup::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::ValueMessageBuilder & messageBuilder) const
{
  bool present = true;

  if(! isMandatory())
  {
    present = pmap.checkNextField();
  }

  if(present)
  {
    if(!segmentBody_)
    {
      decoder.reportFatal("[ERR U08}", "Segment not defined for Group instruction.");
    }
    if(messageBuilder.getApplicationType() != segmentBody_->getApplicationType())
    {
//      std::cout << "Decoding group into new segment: " << segmentBody_->getApplicationType() << std::endl;
      Messages::ValueMessageBuilder & groupBuilder(
        messageBuilder.startGroup(
          identity_,
          segmentBody_->getApplicationType(),
          segmentBody_->getApplicationTypeNamespace(),
          segmentBody_->fieldCount()));

      decoder.decodeGroup(source, segmentBody_, groupBuilder);
      messageBuilder.endGroup(
        identity_,
        groupBuilder);
    }
    else
    {
//      std::cout << "Folding group into parent segment: " << segmentBody_->getApplicationType() << std::endl;
      // Because the application types match,
      // the group fields are decoded directly into to the current
      // field set.  As a result the group "disappears" completely
      // from the application message.  This is a good thing.  Groups
      // are an artifact of the template used to encode the message
      // rather than being an attribute of the actual message being
      // encoded.  In fact, the same message encoded with different
      // templates could be transmitted with different sets of fields
      // in groups.
      decoder.decodeGroup(source, segmentBody_, messageBuilder);
    }
  }
}

void
FieldInstructionGroup::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::MessageAccessor & messageAccessor) const
{
  if(!segmentBody_)
  {
    encoder.reportFatal("[ERR U08}", "Segment not defined for Group instruction.");
  }
  // retrieve the field corresponding to this group
  // Note that applications may support merging groups
  // by returning true from getGroup but using the same accessor.
  const Messages::MessageAccessor * group;
  if(messageAccessor.getGroup(*identity_, group))
  {
    if(! isMandatory())
    {
      pmap.setNextField(true);
    }
    encoder.encodeGroup(destination, segmentBody_, *group);
    messageAccessor.endGroup(*identity_, group);
  }
  else
  {
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U01]", "Missing mandatory group.");
      encoder.encodeGroup(destination, segmentBody_, messageAccessor);
    }
    else
    {
      pmap.setNextField(false);
    }
  }
}

void
FieldInstructionGroup::interpretValue(const std::string & /*value*/)
{
  throw TemplateDefinitionError("[ERR U11] Value not needed by Group instruction.");
}

size_t
FieldInstructionGroup::fieldCount(const SegmentBody & parent)const
{
  if(parent.getApplicationType() == segmentBody_->getApplicationType())
  {
    return segmentBody_->fieldCount();
  }
  else
  {
    return 1;
  }
}

void
FieldInstructionGroup::indexDictionaries(
  DictionaryIndexer & indexer,
  const std::string & dictionaryName,
  const std::string & typeName,
  const std::string & typeNamespace)
{
  if(segmentBody_)
  {
    segmentBody_->indexDictionaries(indexer, dictionaryName,typeName, typeNamespace);
  }
}

ValueType::Type
FieldInstructionGroup::fieldInstructionType()const
{
  return ValueType::GROUP;
}

void
FieldInstructionGroup::displayBody(std::ostream & output, size_t indent)const
{
  segmentBody_->display(output, indent);
}
