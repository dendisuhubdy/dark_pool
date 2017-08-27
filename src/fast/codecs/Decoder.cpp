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

#include "Decoder.h"
#include <Codecs/DataSource.h>
#include <Codecs/PresenceMap.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/FieldInstruction.h>
#include <Messages/ValueMessageBuilder.h>
#include <Common/Profiler.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

Decoder::Decoder(Codecs::TemplateRegistryPtr registry)
: Context(registry)
{
}

//Decoder::Decoder()
//{
//}

void
Decoder::decodeMessage(
   DataSource & source,
   Messages::ValueMessageBuilder & messageBuilder)
{
  PROFILE_POINT("decode");
  source.beginMessage();

  Codecs::PresenceMap pmap(getTemplateRegistry()->presenceMapBits());
  if(this->verboseOut_)
  {
    pmap.setVerbose(verboseOut_);
  }

  static const std::string pmp("PMAP");
  source.beginField(pmp);
  pmap.decode(source);

  static const std::string tid("templateID");
  source.beginField(tid);
  if(pmap.checkNextField())
  {
    template_id_t id;
    FieldInstruction::decodeUnsignedInteger(source, *this, id, tid);
    setTemplateId(id);
  }
  if(verboseOut_)
  {
    (*verboseOut_) << "Template ID: " << getTemplateId() << std::endl;
  }
  Codecs::TemplateCPtr templatePtr;
  if(getTemplateRegistry()->getTemplate(templateId_, templatePtr))
  {
    if(templatePtr->getReset())
    {
      reset(false);
    }
    Messages::ValueMessageBuilder & bodyBuilder(
      messageBuilder.startMessage(
        templatePtr->getApplicationType(),
        templatePtr->getApplicationTypeNamespace(),
        templatePtr->fieldCount()));

    decodeSegmentBody(source, pmap, templatePtr, bodyBuilder);
    if(templatePtr->getIgnore())
    {
      messageBuilder.ignoreMessage(bodyBuilder);
    }
    else
    {
      messageBuilder.endMessage(bodyBuilder);
    }
  }
  else if(templateId_ == SCPResetTemplateId)
  {
    reset(false);
  }
  else
  {
    std::string error =  "Unknown template ID:";
    error += boost::lexical_cast<std::string>(getTemplateId());
    reportError("[ERR D9]", error);
  }
  return;
}

void
Decoder::decodeNestedTemplate(
   DataSource & source,
   Messages::ValueMessageBuilder & messageBuilder,
   Messages::FieldIdentityCPtr & identity)
{
  Codecs::PresenceMap pmap(getTemplateRegistry()->presenceMapBits());
  if(this->verboseOut_)
  {
    pmap.setVerbose(verboseOut_);
  }

  static const std::string pmp("PMAP");
  source.beginField(pmp);
  pmap.decode(source);

  static const std::string tid("templateID");
  source.beginField(tid);
  if(pmap.checkNextField())
  {
    template_id_t id;
    FieldInstruction::decodeUnsignedInteger(source, *this, id, tid);
    setTemplateId(id);
  }
  if(verboseOut_)
  {
    (*verboseOut_) << "Nested Template ID: " << getTemplateId() << std::endl;
  }
  Codecs::TemplateCPtr templatePtr;
  if(getTemplateRegistry()->getTemplate(getTemplateId(), templatePtr))
  {
    if(templatePtr->getReset())
    {
      reset(false);
    }
    Messages::ValueMessageBuilder & groupBuilder(
      messageBuilder.startGroup(
        identity,
        templatePtr->getApplicationType(),
        templatePtr->getApplicationTypeNamespace(),
        templatePtr->fieldCount()));

    decodeSegmentBody(source, pmap, templatePtr, groupBuilder);
    messageBuilder.endGroup(identity, groupBuilder);
  }
  else
  {
    std::string error =  "Unknown template ID:";
    error += boost::lexical_cast<std::string>(getTemplateId());
    reportError("[ERR D9]", error);
  }
  return;
}

void
Decoder::decodeGroup(
  DataSource & source,
  const Codecs::SegmentBodyCPtr & group,
  Messages::ValueMessageBuilder & messageBuilder)
{
  size_t presenceMapBits = group->presenceMapBitCount();
  Codecs::PresenceMap pmap(presenceMapBits);
  if(this->verboseOut_)
  {
    pmap.setVerbose(verboseOut_);
  }

  if(presenceMapBits > 0)
  {
    static const std::string pm("PMAP");
    source.beginField(pm);
    pmap.decode(source);
  }
// for debugging:  pmap.setVerbose(source.getEcho());
  decodeSegmentBody(source, pmap, group, messageBuilder);
}

void
Decoder::decodeSegmentBody(
  DataSource & source,
  Codecs::PresenceMap & pmap,
  const Codecs::SegmentBodyCPtr & segment,
  Messages::ValueMessageBuilder & messageBuilder)
{
  size_t instructionCount = segment->size();
  for( size_t nField = 0; nField < instructionCount; ++nField)
  {
    PROFILE_POINT("decode field");
    const Codecs::FieldInstructionCPtr & instruction = segment->getInstruction(nField);
    if(verboseOut_)
    {
      (*verboseOut_) <<std::endl << "Decode instruction[" <<nField << "]: " << instruction->getIdentity()->name() << std::endl;
    }
    source.beginField(instruction->getIdentity()->name());
    (void)instruction->decode(source, pmap, *this, messageBuilder);
  }
}
