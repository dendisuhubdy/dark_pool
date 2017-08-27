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
#include "TemplateRegistry.h"
#include <Codecs/Template.h>
#include <Codecs/DictionaryIndexer.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

TemplateRegistry::TemplateRegistry()
: presenceMapBits_(1) // every template requires 1 bit for the template ID
, dictionarySize_(0)
, maxFieldCount_(0)
{
}

TemplateRegistry::TemplateRegistry(
  size_t pmapBits,
  size_t fieldCount,
  size_t dictionarySize)
: presenceMapBits_(pmapBits)
, dictionarySize_(dictionarySize)
, maxFieldCount_(fieldCount)
{

}


void
TemplateRegistry::finalize()
{
  // Give every tmplate a chance to finalize itself.
  for(MutableTemplates::iterator mit = mutableTemplates_.begin();
    mit != mutableTemplates_.end();
    ++mit)
  {
    (*mit)->finalize(*this);
  }

  DictionaryIndexer indexer;
  for(MutableTemplates::iterator mit = mutableTemplates_.begin();
    mit != mutableTemplates_.end();
    ++mit)
  {
    (*mit)->indexDictionaries(
      indexer,
      dictionaryName_,
      "", // typeref n/a at <templates> level
      ""); // typeNs
  }
  dictionarySize_ = indexer.size();

  presenceMapBits_ = 1;
  maxFieldCount_ = 0;
  for(TemplateIdMap::const_iterator it = templates_.begin();
    it != templates_.end();
    ++it)
  {
    size_t bits = it->second->presenceMapBitCount();
    if(bits > presenceMapBits_)
    {
      presenceMapBits_ = bits;
    }
    size_t fieldCount = it->second->fieldCount();
    if(fieldCount > maxFieldCount_)
    {
      maxFieldCount_ = fieldCount;
    }
  }
}



void
TemplateRegistry::addTemplate(TemplatePtr value)
{
  template_id_t id = value->getId();
  if(id != 0)
  {
    templates_[id] = value;
  }
  std::string name;
  value->qualifyName(name);
  if(!name.empty())
  {
    namedTemplates_[name] = value;
  }
  mutableTemplates_.push_back(value);
  // TODO: resolve templateRefs before calculating presence map bits.
  //       but that must be deferred to "finalize"
  size_t bits = value->presenceMapBitCount();
  if(bits > presenceMapBits_)
  {
    presenceMapBits_ = bits;
  }
}

size_t
TemplateRegistry::size()const
{
  return templates_.size();
}

bool
TemplateRegistry::getTemplate(template_id_t templateId, TemplateCPtr & valueFound)const
{
  TemplateIdMap::const_iterator it = templates_.find(templateId);
  if(it == templates_.end())
  {
    return false;
  }
  valueFound = it->second;
  return bool(valueFound);
}

bool
TemplateRegistry::findNamedTemplate(
  const std::string & templateName,
  const std::string & templateNamespace,
  TemplateCPtr & valueFound)const
{
  std::string name;
  Template::qualifyName(name, templateName, templateNamespace);
  TemplateNameMap::const_iterator it = namedTemplates_.find(name);
  if(it == namedTemplates_.end())
  {
    return false;
  }
  valueFound = it->second;
  return bool(valueFound);
}

bool
TemplateRegistry::findNamedTemplate(
  const std::string & templateName,
  const std::string & templateNamespace,
  TemplatePtr & valueFound)
{
  bool found = false;
  for(MutableTemplates::iterator mit = mutableTemplates_.begin();
    !found && mit != mutableTemplates_.end();
    ++mit)
  {
    if(templateName == (*mit)->getTemplateName() && templateNamespace == (*mit)->getTemplateNamespace())
    {
      valueFound = *mit;
      found = true;
    }
  }
  return found;
}

void
TemplateRegistry::display(std::ostream & output, size_t indent) const
{
  std::string indentString(indent, ' ');
  output << std::endl << indentString << "<templates";

  if(!templateNamespace_.empty())
  {
    output << " templateNs=\"" << templateNamespace_ << "\"";
  }
  if(!namespace_.empty())
  {
    output << " ns=\"" << namespace_ << "\"";
  }
  if(!dictionaryName_.empty())
  {
    output << " dictionary=\"" << dictionaryName_ << "\"";
  }

  output << ">";
  for(size_t nTemplate = 0; nTemplate < mutableTemplates_.size(); ++nTemplate)
  {
    mutableTemplates_[nTemplate]->display(output, indent + 2);
  }
  output << std::endl << indentString << "</templates>" << std::endl;
}
