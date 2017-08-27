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
#include "MulticastDecoder.h"
#include <Communication/LinkedBuffer.h>
#include <Codecs/MessagePerPacketAssembler.h>

using namespace QuickFAST;
using namespace Codecs;

MulticastDecoder::MulticastDecoder(
  TemplateRegistryPtr templateRegistry,
  const std::string & multicastGroupIP,
  const std::string & listenAddressIP,
  const std::string & bindIP,
  unsigned short portNumber)
: receiver_(multicastGroupIP, listenAddressIP, bindIP, portNumber)
, templateRegistry_(templateRegistry)
, messageLimit_(0)
, strict_(true)
, verboseOut_(0)
{
}

MulticastDecoder::MulticastDecoder(
  TemplateRegistryPtr templateRegistry,
  boost::asio::io_service & ioService,
  const std::string & multicastGroupIP,
  const std::string & listenAddressIP,
  const std::string & bindIP,
  unsigned short portNumber)
: receiver_(ioService, multicastGroupIP, listenAddressIP, bindIP, portNumber)
, templateRegistry_(templateRegistry)
, messageLimit_(0)
, strict_(true)
, verboseOut_(0)
{
}

MulticastDecoder::~MulticastDecoder()
{
}

void
MulticastDecoder::setVerboseOutput(std::ostream & out)
{
  if(assembler_)
  {
    assembler_->decoder().setVerboseOutput(out);
  }
  verboseOut_ = &out;
}

void
MulticastDecoder::disableVerboseOutput()
{
  if(assembler_)
  {
    assembler_->decoder().disableVerboseOutput();
  }
  verboseOut_ = 0;
}

void
MulticastDecoder::setStrict(bool strict)
{
  if(assembler_)
  {
    assembler_->decoder().setStrict(strict);
  }
  strict_ = true;
}

bool
MulticastDecoder::getStrict()const
{
  return strict_;
}

size_t
MulticastDecoder::messageCount() const
{
  if(assembler_)
  {
    return assembler_->messageCount();
  }
  return 0;
}

void
MulticastDecoder::reset()
{
  if(assembler_)
  {
    assembler_->decoder().reset();
  }
}

void
MulticastDecoder::start(
  Messages::ValueMessageBuilder & builder,
  size_t bufferSize /*=1400*/,
  size_t bufferCount /*=2*/)
{
  builder_ = &builder;
  assembler_.reset(new MessagePerPacketAssembler(
    templateRegistry_,
    packetHeaderAnalyzer_,
    messageHeaderAnalyzer_,
    builder));
  assembler_->setMessageLimit(messageLimit_);
  assembler_->decoder().setStrict(strict_);
  if(verboseOut_ != 0)
  {
    assembler_->decoder().setVerboseOutput(*verboseOut_);
  }

  receiver_.start(*assembler_, bufferSize, bufferCount);
}

void
MulticastDecoder::start(
  Messages::ValueMessageBuilder & builder,
  HeaderAnalyzer & packetHeaderAnalyzer,
  size_t bufferSize /*=1400*/,
  size_t bufferCount /*=2*/)
{
  builder_ = &builder;
  assembler_.reset(new MessagePerPacketAssembler(
    templateRegistry_,
    packetHeaderAnalyzer,
    messageHeaderAnalyzer_,
    builder));
  assembler_->setMessageLimit(messageLimit_);
  assembler_->decoder().setStrict(strict_);
  if(verboseOut_ != 0)
  {
    assembler_->decoder().setVerboseOutput(*verboseOut_);
  }

  receiver_.start(*assembler_, bufferSize, bufferCount);
}

void
MulticastDecoder::start(
  Messages::ValueMessageBuilder & builder,
  HeaderAnalyzer & packetHeaderAnalyzer,
  HeaderAnalyzer & messageHeaderAnalyzer,
  size_t bufferSize /*=1400*/,
  size_t bufferCount /*=2*/)
{
  builder_ = &builder;
  assembler_.reset(new MessagePerPacketAssembler(
    templateRegistry_,
    packetHeaderAnalyzer,
    messageHeaderAnalyzer,
    builder));
  assembler_->setMessageLimit(messageLimit_);
  assembler_->decoder().setStrict(strict_);
  if(verboseOut_ != 0)
  {
    assembler_->decoder().setVerboseOutput(*verboseOut_);
  }

  receiver_.start(*assembler_, bufferSize, bufferCount);
}

