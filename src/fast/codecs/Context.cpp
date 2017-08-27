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

#include "Context.h"
#include <Codecs/TemplateRegistry.h>
#include <Common/Exceptions.h>
#include <Messages/FieldIdentity.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

Context::Context(Codecs::TemplateRegistryCPtr registry)
: verboseOut_(0)
, logOut_(0)
, templateRegistry_(registry)
, templateId_(~0U)
, strict_(true)
, indexedDictionarySize_(registry->dictionarySize())
//, indexedDictionary_(new Messages::FieldCPtr[indexedDictionarySize_])
, indexedDictionary_(new Value[indexedDictionarySize_])
{
}

Context::~Context()
{
}

void
Context::reset(bool resetTemplateId /*= true*/)
{
  for(size_t nDict = 0; nDict < indexedDictionarySize_; ++nDict)
  {
    indexedDictionary_[nDict].erase();
  }
  if(resetTemplateId)
  {
    templateId_ = ~0U;
  }
}


bool
Context::findTemplate(const std::string & name, const std::string & nameSpace, TemplateCPtr & result) const
{
  return templateRegistry_->findNamedTemplate(
        name,
        nameSpace,
        result);
}

void
Context::logMessage(const std::string & message)
{
  if(logOut_)
  {
    (*logOut_) << message << std::endl;
  }
}

void
Context::reportWarning(const std::string & errorCode, const std::string & message)
{
  if(logOut_)
  {
    (*logOut_) << errorCode << ' ' << message << std::endl;
  }
}

void
Context::reportWarning(
  const std::string & errorCode,
  const std::string & message,
  const Messages::FieldIdentity & identity)
{
  if(logOut_)
  {
    (*logOut_) << errorCode << ' ' << message  << " Field: " << identity.name() << std::endl;
  }
}

void
Context::reportWarning(
  const std::string & errorCode,
  const std::string & message,
  const std::string & name)
{
  if(logOut_)
  {
    (*logOut_) << errorCode << ' ' << message  << " Field: " << name << std::endl;
  }
}

void
Context::reportError(const std::string & errorCode, const std::string & message)
{
  if(!strict_)
  {
    if(errorCode == "[ERR D2]")
    {
      return;
    }
  }
  throw EncodingError(errorCode + ' ' + message);
}

void
Context::reportError(
  const std::string & errorCode,
  const std::string & message,
  const Messages::FieldIdentity & identity)
{
  throw EncodingError(errorCode + ' ' + message + " Field: " + identity.name());
}

void
Context::reportError(
  const std::string & errorCode,
  const std::string & message,
  const std::string & name)
{
  throw EncodingError(errorCode + ' ' + message + " Field: " + name);
}

void
Context::reportFatal(const std::string & errorCode, const std::string & message)
{
  throw EncodingError(errorCode + ' ' +  message);
}

void
Context::reportFatal(
  const std::string & errorCode,
  const std::string & message,
  const Messages::FieldIdentity & identity)
{
  throw EncodingError(errorCode + ' ' + message + " Field: " + identity.name());
}

void
Context::reportFatal(
  const std::string & errorCode,
  const std::string & message,
  const std::string & name)
{
  throw EncodingError(errorCode + ' ' + message + " Field: " + name);
}

