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
#include "FieldByteVector.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldByteVector::FieldByteVector(const std::string & value)
  : Field(ValueType::BYTEVECTOR, true)
{
  string_ = value;
}


FieldByteVector::FieldByteVector(const uchar * buffer, size_t length)
  : Field(ValueType::BYTEVECTOR, true)
{
  string_.assign(buffer, length);
}

FieldByteVector::FieldByteVector()
  : Field(ValueType::BYTEVECTOR, false)
{
}

FieldByteVector::~FieldByteVector()
{
}

bool
FieldByteVector::isString() const
{
  return true;
}

const StringBuffer &
FieldByteVector::toByteVector() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return string_;
}


FieldCPtr
FieldByteVector::create(const std::string & value)
{
  return new FieldByteVector(value);
}

FieldCPtr
FieldByteVector::create(const uchar * buffer, size_t length)
{
  return new FieldByteVector(buffer, length);
}

FieldCPtr
FieldByteVector::createNull()
{
  return new FieldByteVector;
}

