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
#include "MessageBuilder.h"
#include <Messages/FieldInt64.h>
#include <Messages/FieldUInt64.h>
#include <Messages/FieldInt32.h>
#include <Messages/FieldUInt32.h>
#include <Messages/FieldInt16.h>
#include <Messages/FieldUInt16.h>
#include <Messages/FieldInt8.h>
#include <Messages/FieldUInt8.h>
#include <Messages/FieldDecimal.h>
#include <Messages/FieldString.h>
#include <Messages/FieldAscii.h>
#include <Messages/FieldUtf8.h>
#include <Messages/FieldByteVector.h>

using namespace QuickFAST;
using namespace Messages;

MessageBuilder::MessageBuilder()
: vout_(0)
{
}

void MessageBuilder::addValue(FieldIdentityCPtr & identity, ValueType::Type type, const int64 value)
{
  if(vout_)
  {
    (*vout_)
      << "Assign: " << identity->name() << " = " << value << std::endl;
  }
  FieldCPtr field(FieldInt64::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(FieldIdentityCPtr & identity, ValueType::Type type, const uint64 value)
{
  if(vout_)
  {
    (*vout_)
      << "Assign: " << identity->name() << " = " << value << std::endl;
  }
  FieldCPtr field(FieldUInt64::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(FieldIdentityCPtr & identity, ValueType::Type type, const int32 value)
{
  if(vout_)
  {
    (*vout_)
      << "Assign: " << identity->name() << " = " << value << std::endl;
  }
  FieldCPtr field(FieldInt32::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(FieldIdentityCPtr & identity, ValueType::Type type, const uint32 value)
{
  if(vout_)
  {
    (*vout_)
      << "Assign: " << identity->name() << " = " << value << std::endl;
  }
  FieldCPtr field(FieldUInt32::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(FieldIdentityCPtr & identity, ValueType::Type type, const int16 value)
{
  if(vout_)
  {
    (*vout_)
      << "Assign: " << identity->name() << " = " << value << std::endl;
  }
  FieldCPtr field(FieldInt16::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(FieldIdentityCPtr & identity, ValueType::Type type, const uint16 value)
{
  if(vout_)
  {
    (*vout_)
      << "Assign: " << identity->name() << " = " << value << std::endl;
  }
  FieldCPtr field(FieldUInt16::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(FieldIdentityCPtr & identity, ValueType::Type type, const int8 value)
{
  if(vout_)
  {
    (*vout_)
      << "Assign: " << identity->name() << " = " << std::hex << (0xFF & (static_cast<unsigned short>(value))) << std::dec << std::endl;
  }
  FieldCPtr field(FieldInt8::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(FieldIdentityCPtr & identity, ValueType::Type type, const uchar value)
{
  if(vout_)
  {
    (*vout_)
      << "Assign: " << identity->name() << " = " << std::hex << static_cast<unsigned short>(value) << std::dec << std::endl;
  }
  FieldCPtr field(FieldUInt8::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(FieldIdentityCPtr & identity, ValueType::Type type, const Decimal& value)
{
  if(vout_)
  {
    (*vout_)
      << "Assign: " << identity->name() << " = " << value << std::endl;
  }
  FieldCPtr field(FieldDecimal::create(value));
  addField(identity, field);
}
void MessageBuilder::addValue(FieldIdentityCPtr & identity, ValueType::Type type, const unsigned char * value, size_t length)
{
  if(vout_)
  {
    (*vout_)
      << "Assign: " << identity->name() << " = " << std::string(reinterpret_cast<const char *>(value), length) << std::endl;
  }
  switch (type)
  {
  case ValueType::ASCII:
    addField(identity, FieldAscii::create(value, length));
    break;
  case ValueType::UTF8:
    addField(identity, FieldUtf8::create(value, length));
    break;
  case ValueType::BYTEVECTOR:
    addField(identity, FieldByteVector::create(value, length));
    break;
  default:
    addField(identity, FieldString::create(value, length));
  }
}
