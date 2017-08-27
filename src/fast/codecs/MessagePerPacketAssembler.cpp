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
#include <Communication/Receiver.h>
#include "MessagePerPacketAssembler.h"
#include <Messages/ValueMessageBuilder.h>
#include <Codecs/Decoder.h>

using namespace QuickFAST;
using namespace Codecs;

MessagePerPacketAssembler::MessagePerPacketAssembler(
      TemplateRegistryPtr templateRegistry,
      HeaderAnalyzer & packetHeaderAnalyzer,
      HeaderAnalyzer & messageHeaderAnalyzer,
      Messages::ValueMessageBuilder & builder)
  : BasePacketAssembler(
    templateRegistry, packetHeaderAnalyzer,
    messageHeaderAnalyzer,
    builder)
{
}

MessagePerPacketAssembler::~MessagePerPacketAssembler()
{
}

bool
MessagePerPacketAssembler::serviceQueue(Communication::Receiver & receiver)
{
  bool result = true;
  Communication::LinkedBuffer * buffer = receiver.getBuffer(false);
  while(result && buffer != 0)
  {
    try
    {
      result = decodeBuffer(buffer->get(), buffer->used());
    }
    catch(const std::exception &ex)
    {
      receiver.releaseBuffer(buffer);
      buffer = 0;
      result = reportDecodingError(ex.what());
      reset();
    }
    if(buffer != 0)
    {
      receiver.releaseBuffer(buffer);
      buffer = 0;
    }
    if(result)
    {
      buffer = receiver.getBuffer(false);
    }
  }
  return result;
}

