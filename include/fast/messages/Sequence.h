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
#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "Sequence_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Messages/FieldSet.h>

namespace QuickFAST{
  namespace Messages{
    /// Container for a sequence of field groups
    class Sequence
    {
    public:
      /// @brief Each entry is a field set
      typedef FieldSetCPtr Entry;
      /// @brief We store them in a vector
      typedef std::vector<Entry> Entries;
      /// @brief We support iteration through the sequence
      typedef Entries::const_iterator const_iterator;

    public:
      /// @brief construct an empty sequence
      Sequence(
        Messages::FieldIdentityCPtr & lengthFieldIdentity,
        size_t sequenceLength)
        : lengthIdentity_(lengthFieldIdentity)
      {
        this->entries_.reserve(sequenceLength);
      }

      ~Sequence()
      {
      }

      /// @brief get the identity of the sequence's length field
      const Messages::FieldIdentityCPtr & getLengthIdentity() const
      {
        return lengthIdentity_;
      }

      /// @brief Set the application data type associated with this sequence.
      void setApplicationType(const std::string & type)
      {
        applicationType_ = type;
      }

      /// @brief Get the application data type associated with this sequence.
      const std::string & getApplicationType()const
      {
        return applicationType_;
      }

      /// @brief Add a new group to the sequence
      void addEntry(FieldSetCPtr fieldSet)
      {
        entries_.push_back(fieldSet);
      }

      /// @brief How many entries are in the sequence.
      size_t size()const
      {
        return entries_.size();
      }

      /// @brief access an entry by index.
      const FieldSetCPtr & operator [](size_t index) const
      {
        return entries_[index];
      }

      /// @brief access an entry by index.
      FieldSetCPtr & operator [](size_t index)
      {
        return entries_[index];
      }

      /// @brief Support iteration from the beginning
      const_iterator begin() const
      {
        return entries_.begin();
      }

      /// @brief Support iteration to the end
      const_iterator end() const
      {
        return entries_.end();
      }

    private:
      Sequence();
      Sequence(const Sequence&);
      Sequence& operator=(const Sequence&);
    private:
      std::string applicationType_;
      Messages::FieldIdentityCPtr lengthIdentity_;
      Entries entries_;
    };
  }
}

#endif // SEQUENCE_H
