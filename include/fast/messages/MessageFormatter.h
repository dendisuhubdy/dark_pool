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
#ifndef MESSAGEFORMATTER_H
#define MESSAGEFORMATTER_H
#include <Common/QuickFAST_Export.h>
#include <Messages/FieldIdentity_fwd.h>
#include <Messages/Message_fwd.h>
#include <Messages/Field_fwd.h>

namespace QuickFAST
{
  namespace Messages
  {
    ///@brief Format message for display in human-readable format
    class QuickFAST_Export MessageFormatter
    {
    public:
      /// @brief construct
      /// @param out is the stream to which formatted data will be written
      MessageFormatter(std::ostream & out);
      ~MessageFormatter();

      /// @brief Format the entire message
      /// @param message is the message to be formatted
      void formatMessage(const Messages::Message & message);

      /// @brief Format a sequence
      /// @param identity identifies this sequence
      /// @param field is the field object containing the sequence
      void formatSequence(
        const Messages::FieldIdentityCPtr & identity,
        const Messages::FieldCPtr & field);
      /// @brief Format a group
      /// @param identity identifies this group
      /// @param field is the field object containing the group
      void formatGroup(
        const Messages::FieldIdentityCPtr & identity,
        const Messages::FieldCPtr & field);

      /// @brief Format a single field
      /// @param field contains the field to be formatted
      void displayFieldValue(const Messages::FieldCPtr & field);
    private:
      MessageFormatter & operator =(const MessageFormatter &); // do not autogenerate assignment operator
    private:
      void newline();
    private:
      std::ostream & out_;
      size_t indent_;
    };
  }
}
#endif /* MESSAGEFORMATTER_H */
