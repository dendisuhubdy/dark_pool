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
#include "FieldString.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldString::FieldString(const std::string & value)
  : Field(ValueType::ASCII, true)
{
  string_ = value;
}

FieldString::FieldString(const uchar * value, size_t length)
  : Field(ValueType::ASCII, true)
{
  string_.assign(value, length);
}

FieldString::FieldString()
  : Field(ValueType::ASCII, false)
{
}

FieldString::~FieldString()
{
}

bool
FieldString::isString() const
{
  return true;
}

const StringBuffer &
FieldString::toAscii() const
{
  return toString();
}

const StringBuffer &
FieldString::toUtf8() const
{
  return toString();
}

const StringBuffer &
FieldString::toByteVector() const
{
  return toString();
}

FieldCPtr
FieldString::create(const std::string & value)
{
  return new FieldString(value);
}

FieldCPtr
FieldString::create(const uchar * buffer, size_t length)
{
  return new FieldString(buffer, length);
}

FieldCPtr
FieldString::createNull()
{
  return new FieldString;
}

