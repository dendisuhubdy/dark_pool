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
#include "FieldIdentity.h"

#ifdef _MSC_VER
#pragma warning(disable:4355) // disable warning C4355: 'this' : used in base member initializer list
#endif
using namespace QuickFAST;
using namespace Messages;

static
std::string anonName(void * address)
{
  return boost::lexical_cast<std::string>(address);
}

FieldIdentity::FieldIdentity()
  : localName_(anonName(this))
  , refcount_(0)
{
  qualifyName();
}

FieldIdentity::FieldIdentity(
  const std::string & name,
  const std::string & fieldNamespace /* = ""*/,
  const field_id_t & id /* = ""*/)
  : localName_(name)
  , fieldNamespace_(fieldNamespace)
  , id_(id)
  , refcount_(0)
{
  qualifyName();
}


FieldIdentity::~FieldIdentity()
{
}

void
FieldIdentity::freeFieldIdentity()const
{
  delete this;
}

void
FieldIdentity::display(std::ostream & output)const
{
  output << " name=\"" << localName_;
  if(!fieldNamespace_.empty())
  {
    output << "\" ns=\"" << fieldNamespace_;
  }
  if(!id_.empty())
  {
    output << "\" id=\"" << id_;
  }
  output << '"';
}
