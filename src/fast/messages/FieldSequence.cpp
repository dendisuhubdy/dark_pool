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
#include "FieldSequence.h"
#include <Messages/Sequence.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldSequence::FieldSequence(Messages::SequenceCPtr sequence)
  : Field(ValueType::SEQUENCE, true)
  , sequence_(sequence)
{
}

FieldSequence::~FieldSequence()
{
}

uint32
FieldSequence::toUInt32()const
{
  return static_cast<uint32>(sequence_->size());
}

const Messages::SequenceCPtr &
FieldSequence::toSequence() const
{
  return sequence_;
}

FieldCPtr
FieldSequence::create(Messages::SequenceCPtr sequence)
{
  return new FieldSequence(sequence);
}

bool
FieldSequence::operator == (const Field & rhs) const
{
  if(getType() != rhs.getType())
  {
    return false;
  }
///@TODO  int todo_improve_comparison;
  return true;
}

void
FieldSequence::valueToStringBuffer() const
{
  static const unsigned char msg[] = "Sequence: ";
  string_.assign(msg, sizeof(msg)-1);
}


