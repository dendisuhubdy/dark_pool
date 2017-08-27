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
#ifndef MESSAGEACCESSOR_H
#define MESSAGEACCESSOR_H
#include <Messages/MessageField_fwd.h>
#include <Messages/FieldIdentity_fwd.h>
#include <Messages/Field.h>

namespace QuickFAST
{
  namespace Messages
  {
    /// @brief Internal representation of a set of fields to be encoded or decoded.
    class QuickFAST_Export MessageAccessor
    {
    public:
      /// @brief construct
      /// tid = the template id to use to encode this message
      explicit MessageAccessor(template_id_t tid = template_id_t(-1));

      /// @brief Virtual destructor
      virtual ~MessageAccessor();

      /// @brief pick template to use for encoding next message
      ///
      /// @param[out] tid is the template chosen.
      /// @returns false if no more data to encode.
      virtual bool pickTemplate(template_id_t & tid);


      /// @brief Is the specified field present in this set?
      /// @param identity Identifies the field.
      /// @returns true if the field is present; false if the field is
      ///               unknown or doesn't currently have a value in this set.
      virtual bool isPresent(const FieldIdentity & identity)const = 0;

      /// @brief Get a field from the application record.
      ///
      /// @param identity identifies this field
      /// @param type is the type of data requested
      /// @param value is the value to be returned.
      virtual bool getUnsignedInteger(const FieldIdentity & identity, ValueType::Type type, uint64 & value)const = 0;

      /// @brief Get a field from the application record.
      ///
      /// @param identity identifies this field
      /// @param type is the type of data requested
      /// @param value is the value to be returned.
      virtual bool getSignedInteger(const FieldIdentity & identity, ValueType::Type type, int64 & value)const = 0;

      /// @brief Get a field from the application record.
      ///
      /// @param identity identifies this field
      /// @param type is the type of data requested
      /// @param value is the value to be returned.
      virtual bool getDecimal(const FieldIdentity & identity, ValueType::Type type, Decimal & value)const = 0;

      /// @brief Get a field from the application record.
      ///
      /// @param identity identifies this field
      /// @param type is the type of data requested
      /// @param value is the value to be returned.
      virtual bool getString(const FieldIdentity & identity, ValueType::Type type, const StringBuffer *& value)const = 0;

      /// @brief Start retrieving data from a group
      ///
      /// @param identity identifies the group field
      /// @param[out] group is set to point to an object that accesses the group data
      virtual bool getGroup(const FieldIdentity & identity, const MessageAccessor *& group)const = 0;
      /// @brief End retrieving data from a group
      ///
      /// Not pure because a common case is to ignore this.
      /// @param identity echos the corresponding parameter to getGroup
      /// @param group echos the corresponding result of the call to getGroup
      virtual void endGroup(const FieldIdentity & identity, const MessageAccessor * group)const;

      /// @brief Start accessng a sequence by determining how many sequence elements appear in the application data
      ///
      /// @param identity identifies the sequence field.
      /// @param[out] length returns the number of entries in the sequence
      /// @returns true if the sequence is present
      virtual bool getSequenceLength(const FieldIdentity & identity, size_t & length)const = 0;

      /// @brief Start decoding a sequence entry
      ///
      /// @param identity identifies the sequence field.
      /// @param index specifies which entry in the sequence
      /// @param[out] entry is set to point to the object that accesses the entry's data
      /// @returns true unless something is broken in the seequence (or the index is > length)
      virtual bool getSequenceEntry(const FieldIdentity & identity, size_t index, const MessageAccessor *& entry)const = 0;

      /// @brief Finish decoding a sequence entry
      ///
      /// Not pure because a common case is to ignore this.
      /// @param identity echos the corresponding parameter to getSequenceEntry()
      /// @param index echos the corresponding parameter to getSequenceEntry();
      /// @param entry is the result returned by to getSequenceEntry()
      virtual void endSequenceEntry(const FieldIdentity & identity, size_t index, const MessageAccessor * entry)const;

      /// @brief Finish accessing a sequence
      ///
      /// Not pure because a common case is to ignore this.
      /// @param identity echos the corersponding parameter to getSequenceLength
      virtual void endSequence(const FieldIdentity & identity)const;

      /// @brief get the application type associated with
      /// this set of fields via typeref.
      virtual const std::string & getApplicationType()const = 0;

      /// @brief get the namespace for the application type
      virtual const std::string & getApplicationTypeNs()const = 0;

    private:
      MessageAccessor & operator=(MessageAccessor &); // no autogenerated assignment
    private:
      template_id_t & tid_;
    };
  }
}
#endif // MESSAGEACCESSOR_H
