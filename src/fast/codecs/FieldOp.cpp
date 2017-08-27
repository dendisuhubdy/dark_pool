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
#include "FieldOp.h"
#include <Codecs/DictionaryIndexer.h>
#include <Codecs/Context.h>
#include <Common/Exceptions.h>

using namespace QuickFAST;
using namespace Codecs;

FieldOp::FieldOp()
: valueIsDefined_(false)
, dictionaryIndex_(0)
, dictionaryIndexValid_(false)
, pmapBit_(0)
, pmapBitValid_(false)
{
}

void
FieldOp::indexDictionaries(
  DictionaryIndexer & indexer,
  const std::string & dictionaryName,
  const std::string & typeName,
  const std::string & typeNamespace,
  const std::string & fieldName,
  const std::string & fieldNamespace)
{
  if(usesDictionary())
  {
    std::string name = dictionaryName;
    if(!dictionaryName_.empty())
    {
      name = dictionaryName_;
    }
    std::string key = fieldName;
    std::string keyNamespace = fieldNamespace;
    if(!key_.empty())
    {
      key = key_;
      keyNamespace = keyNamespace_;
    }
    dictionaryIndex_ = indexer.getIndex(
      name,
      typeName,
      typeNamespace,
      key,
      keyNamespace);
    dictionaryIndexValid_ = true;
  }
}

const std::string &
FieldOp::opName(OpType type)
{
  switch(type)
  {
  case NOP:
    {
      static const std::string nop("nop");
      return nop;
    }
  case CONSTANT:
    {
      static const std::string constant("constant");
      return constant;
    }
  case DEFAULT:
    {
      static const std::string defaultname("default");
      return defaultname;
    }
  case COPY:
    {
      static const std::string copy("copy");
      return copy;
    }
  case DELTA:
    {
      static const std::string delta("delta");
      return delta;
    }
  case INCREMENT:
    {
      static const std::string increment("increment");
      return increment;
    }
  case TAIL:
    {
      static const std::string tail("tail");
      return tail;
    }
  case UNKNOWN:
  default:
    break;
  }
  static const std::string unknown("UNKNOWN");
  return unknown;
}
