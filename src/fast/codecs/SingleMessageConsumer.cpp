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
#include "SingleMessageConsumer.h"
using namespace QuickFAST;
using namespace Codecs;

SingleMessageConsumer::SingleMessageConsumer()
: message_(1)
{
}
SingleMessageConsumer::~SingleMessageConsumer()
{
}

bool
SingleMessageConsumer::consumeMessage(Messages::Message & message)
{
  message_.swap(message);
  return true;
}

bool
SingleMessageConsumer::wantLog(unsigned short /*level*/)
{
  return true;
}

bool
SingleMessageConsumer::logMessage(unsigned short /*level*/, const std::string & logMessage)
{
  std::cout << logMessage << std::endl;
  return true;
}

bool
SingleMessageConsumer::reportDecodingError(const std::string & errorMessage)
{
  std::cout << errorMessage << std::endl;
  return true;
}

bool
SingleMessageConsumer::reportCommunicationError(const std::string & errorMessage)
{
  std::cout << errorMessage << std::endl;
  return true;
}

void
SingleMessageConsumer::decodingStarted()
{
}

void
SingleMessageConsumer::decodingStopped()
{

}

Messages::Message &
SingleMessageConsumer::message()
{
  return message_;
}
