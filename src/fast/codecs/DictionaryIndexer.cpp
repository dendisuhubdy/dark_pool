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
#include "DictionaryIndexer.h"

using namespace QuickFAST;
using namespace Codecs;

DictionaryIndexer::DictionaryIndexer()
: index_(0)
{
}

DictionaryIndexer::~DictionaryIndexer()
{
}


void
DictionaryIndexer:: newTemplate()
{
  templateNames_.clear();
}

size_t
DictionaryIndexer::getIndex(
  const std::string & dictionaryName,
  const std::string & typeName,
  const std::string & typeNamespace,
  const std::string & key,
  const std::string & keyNamespace)
{
  if(dictionaryName.empty() || dictionaryName == "global")
  {
    return getDictionaryIndex(
      globalNames_,
      keyNamespace + '\t' + key);
  }
  else if(dictionaryName == "type")
  {
    return getDictionaryIndex(
      typeNames_,
      typeNamespace + '\t' +typeName + '\t' + keyNamespace + '\t' + key);
  }
  else if(dictionaryName == "template")
  {
    return getDictionaryIndex(
      templateNames_,
      keyNamespace + '\t' + key);
  }
  else
  {
    return getDictionaryIndex(
      qualifiedNames_,
      dictionaryName + '\t' + keyNamespace + '\t' + key);
  }
}

size_t
DictionaryIndexer::getDictionaryIndex(NameToIndex & nameToIndex, const std::string & key)
{
  size_t result = 0;
  NameToIndex::const_iterator it = nameToIndex.find(key);
  if(it != nameToIndex.end())
  {
    result = it->second;
  }
  else
  {
    result = index_++;
    nameToIndex[key] = result;
  }
  return result;
}

size_t
DictionaryIndexer::size()const
{
  return index_;
}
