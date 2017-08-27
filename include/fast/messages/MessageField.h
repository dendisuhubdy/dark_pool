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
#ifndef MESSAGEFIELD_H
#define MESSAGEFIELD_H

///@todo: think about how to get the performance improvement without compromising safety
/// The problem is that copying the identity is expensive (15% on the profiler) but keeping
/// a pointer to the identity gets into object lifetime issues.  The identity was not
/// originally designed to be heap-allocated, counted ptr managed, but maybe it should have
/// been because it's immutable at encode/decode time.
#define SAFE_BUT_SLOW

#include "MessageField_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Messages/Field_fwd.h>
#include <Messages/FieldIdentity.h>
#include <Common/Profiler.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief the representation of a field within a message.
    class QuickFAST_Export MessageField
    {
    public:
      /// @brief Construct from an identity and a typed value.
      MessageField(const FieldIdentityCPtr & identity, const FieldCPtr & field)
        : identity_(identity)
        , field_(field)
      {
      }

      /// @brief copy constructor
      /// @param rhs the source from which to copy
      MessageField(const MessageField & rhs)
        : identity_(rhs.identity_)
        , field_(rhs.field_)
      {
      }

    public:

      /// @brief get the name of the field
      /// @returns the fully qualified field name
      const std::string name()const
      {
        return identity_->name();
      }
      /// @brief get the identity of the field
      /// @returns the identifying information for this field
      const FieldIdentityCPtr & getIdentity()const
      {
        return identity_;
      }

      /// @brief get the value of the field
      /// @returns  a pointer to the Field
      const FieldCPtr & getField()const
      {
        return field_;
      }
    private:
      FieldIdentityCPtr identity_;
      FieldCPtr field_;
    };
  }
}
#endif // MESSAGEFIELD_H
