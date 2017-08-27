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
#include "Types.h"

using namespace QuickFAST;

const std::string &
ValueType::typeName(ValueType::Type type)
{
  switch(type)
  {
  case INT8:
    {
      static const std::string name("int8");
      return name;
    }
  case UINT8:
    {
      static const std::string name("uInt8");
      return name;
    }
  case INT16:
    {
      static const std::string name("int16");
      return name;
    }
  case UINT16:
    {
      static const std::string name("uInt16");
      return name;
    }
  case INT32:
    {
      static const std::string name("int32");
      return name;
    }
  case UINT32:
    {
      static const std::string name("uInt32");
      return name;
    }
  case INT64:
    {
      static const std::string name("int64");
      return name;
    }
  case UINT64:
    {
      static const std::string name("uInt64");
      return name;
    }
  case DECIMAL:
    {
      static const std::string name("decimal");
      return name;
    }
  case EXPONENT:
    {
      static const std::string name("exponent");
      return name;
    }
  case MANTISSA:
    {
      static const std::string name("mantissa");
      return name;
    }
  case ASCII:
    {
      static const std::string name("ascii");
      return name;
    }
  case UTF8:
    {
      static const std::string name("utf8");
      return name;
    }
  case BYTEVECTOR:
    {
      static const std::string name("byteVector");
      return name;
    }
  case SEQUENCE:
    {
      static const std::string name("sequence");
      return name;
    }
  case LENGTH:
    {
      static const std::string name("length");
      return name;
    }
  case GROUP:
    {
      static const std::string name("group");
      return name;
    }
  case TEMPLATEREF:
    {
      static const std::string name("templateRef");
      return name;
    }

  case TYPEREF:
    {
      static const std::string name("typeRef");
      return name;
    }

  default:
    {
      static const std::string name("Unknown");
      return name;
    }
  }
}

