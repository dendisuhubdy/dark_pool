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
#ifndef DICTIONARYINDEXER_H
#define DICTIONARYINDEXER_H
#include <Common/QuickFAST_Export.h>
#include "DictionaryIndexer_fwd.h"

namespace QuickFAST{
  namespace Codecs{
    /// @brief Helps build an index of Dictionary Items
    ///
    /// Allows finding and updating by index rather than by search
    /// Note that this simply assigns index values before encoding/decoding begins.
    /// The actual array of dictionary entries will be created by the Context
    /// when the Encoder or Decoder is created.
    class QuickFAST_Export DictionaryIndexer
    {
    public:
      DictionaryIndexer();
      ~DictionaryIndexer();
      /// @brief tell the indexer when you start a new template.
      void newTemplate();

      /// @brief Get the index for a field
      ///
      /// This will either find an existing index, or create a new one.
      ///
      /// @param dictionaryName identifies the dictionary to be used for this field.
      ///        Special values "", "global", "template", and "type" will be recognized.
      ///        Any other value is a user defined dictionary.
      /// @param typeName names the application type in case dictionary name is "type"
      /// @param typeNamespace namespace to qualifytypeName
      /// @param key is the key to identify the element in the dictionary
      /// @param keyNamespace qualifies the key name.
      size_t getIndex(
        const std::string & dictionaryName,
        const std::string & typeName,
        const std::string & typeNamespace,
        const std::string & key,
        const std::string & keyNamespace);

      /// @brief How many dictionary entries are needed.
      /// @returns a count of dictionary entries.
      size_t size()const;

    private:
      typedef std::map<std::string, size_t> NameToIndex;
      size_t getDictionaryIndex(NameToIndex & nameToIndex, const std::string & key);

      NameToIndex globalNames_;
      NameToIndex templateNames_;
      NameToIndex typeNames_;
      NameToIndex qualifiedNames_;
      size_t index_;
    };
  }
}
#endif // DICTIONARYINDEXER_H
