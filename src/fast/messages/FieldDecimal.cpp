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
#include "FieldDecimal.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldDecimal::FieldDecimal()
  : Field(ValueType::DECIMAL, false)
{
}

FieldDecimal::FieldDecimal(const Decimal & value)
  : Field(ValueType::DECIMAL, true)
{
  signedInteger_ = value.getMantissa();
  exponent_ = value.getExponent();
}

FieldDecimal::FieldDecimal(mantissa_t mantissa, exponent_t exponent)
  : Field(ValueType::DECIMAL, true)
{
  signedInteger_ = mantissa;
  exponent_ = exponent;
}

FieldDecimal::~FieldDecimal()
{
}

const Decimal
FieldDecimal::toDecimal() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return Decimal(signedInteger_, exponent_);
}

FieldCPtr
FieldDecimal::create(const Decimal & value)
{
  return new FieldDecimal(value);
}

FieldCPtr
FieldDecimal::create(mantissa_t mantissa, exponent_t exponent)
{
  return new FieldDecimal(mantissa, exponent);
}

FieldCPtr
FieldDecimal::createNull()
{
  return new FieldDecimal;
}

void
FieldDecimal::valueToStringBuffer() const
{
  std::stringstream buffer;
  buffer << signedInteger_ << "E" << exponent_;
  string_.assign(reinterpret_cast<const unsigned char *>(buffer.str().data()), buffer.str().size());
}
