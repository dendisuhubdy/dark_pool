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
#include "MessageAccessor.h"
#include <Messages/FieldSet.h>
#include <Messages/Sequence.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

MessageAccessor::MessageAccessor(template_id_t tid)
: tid_(tid)
{
}
MessageAccessor::~MessageAccessor()
{
}

bool
MessageAccessor::pickTemplate(template_id_t & tid)
{
  if(tid_ == template_id_t(-1))
  {
    return false;
  }
  tid = tid_;
  tid_ = template_id_t(-1);
  return true;
}


void
MessageAccessor::endGroup(const FieldIdentity & identity, const MessageAccessor * groupAccessor)const
{
}

void
MessageAccessor::endSequence(const FieldIdentity & identity)const
{
}

void
MessageAccessor::endSequenceEntry(const FieldIdentity & identity, size_t index, const MessageAccessor * entryAccessor)const
{
}
