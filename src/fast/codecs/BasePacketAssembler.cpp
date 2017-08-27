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
#include "BasePacketAssembler.h"
#include <Messages/ValueMessageBuilder.h>
#include <Codecs/Decoder.h>

using namespace QuickFAST;
using namespace Codecs;

BasePacketAssembler::BasePacketAssembler(
    TemplateRegistryPtr templateRegistry,
    HeaderAnalyzer & packetHeaderAnalyzer,
    HeaderAnalyzer & messageHeaderAnalyzer,
    Messages::ValueMessageBuilder & builder
) : Communication::Assembler(templateRegistry, builder), 
    packetHeaderAnalyzer_(packetHeaderAnalyzer),
     messageHeaderAnalyzer_(messageHeaderAnalyzer),
    builder_(builder),
    messageCount_(0),
    byteCount_(0),
    messageLimit_(0)
{
}

BasePacketAssembler::~BasePacketAssembler()
{
}

bool
BasePacketAssembler::decodeBuffer(
    const unsigned char *buffer,
    size_t size
)
{
  bool result = true;
  ++messageCount_;
  ++byteCount_ += size;
  if(builder_.wantLog(Common::Logger::QF_LOG_VERBOSE))
  {
    std::stringstream message;
    message << "Received[" << messageCount_ << "]: " << size << " bytes" << std::endl;
    for(size_t i = 0; i < size; ++i)
    {
      message << std::hex << std::setw(2) << std::setfill('0') << (unsigned short)(buffer[i]) << std::setfill(' ') << std::dec << ' ';
      if((i+1)% 32 == 0)
      {
        message << std::endl;
      }
    }
    message << std::endl;
    builder_.logMessage(Common::Logger::QF_LOG_VERBOSE, message.str());
  }
  try
  {
    currentBuffer_ = buffer;
    currentSize_ = size;
    size_t blockSize = 0;
    bool skipBlock = false;
    if(!packetHeaderAnalyzer_.analyzeHeader(*this, blockSize, skipBlock))
    {
      // header must be complete in one packet
      builder_.reportDecodingError("Invalid header in packet.  Ignoring packet.");
      packetHeaderAnalyzer_.reset();
      DataSource::reset();
      currentSize_ = 0;
      currentBuffer_ = 0;
    }
    else
    {
      if(skipBlock)
      {
        DataSource::reset();
        currentSize_ = 0;
        currentBuffer_ = 0;
      }
      else
      {
        // note we apply reset at the packet level. If there are multiple messages per packet
        // the decoder is NOT reset for each one.
        if(reset_)
        {
          decoder_.reset();
        }
        while(bytesAvailable() > 0)
        {
          bool skipMessage = false;
          size_t messageSize = 0;
          if(!messageHeaderAnalyzer_.analyzeHeader(*this, messageSize, skipMessage))
          {
            // header must be complete in one packet
            builder_.reportDecodingError("Invalid message header.  Ignoring remainder of packet.");
            messageHeaderAnalyzer_.reset();
            DataSource::reset();
            currentSize_ = 0;
            currentBuffer_ = 0;
          }
          else if(skipMessage)
          {
            DataSource::reset();
            currentSize_ = 0;
            currentBuffer_ = 0;
          }
          else
          {
            decoder_.decodeMessage(*this, builder_);
          }
        }
      }
    }
  }
  catch (const std::exception &ex)
  {
    result = builder_.reportDecodingError(ex.what());
    reset();
  }
  if(result && messageCount_ > messageLimit_ && messageLimit_ != 0)
  {
    result = false;
  }
  // an aid to debugging
  // memset(const_cast<unsigned char *>(buffer), '\xCD' , size);
  return result;
}

void
BasePacketAssembler::receiverStarted(Communication::Receiver & /*receiver*/)
{
  decoder_.setStrict(strict_);
  if(builder_.wantLog(Common::Logger::QF_LOG_INFO))
  {
    builder_.logMessage(Common::Logger::QF_LOG_INFO, "Receiver started");
  }
}
void
BasePacketAssembler::receiverStopped(Communication::Receiver & /*receiver*/)
{
  if(builder_.wantLog(Common::Logger::QF_LOG_INFO))
  {
    builder_.logMessage(Common::Logger::QF_LOG_INFO, "Receiver stopped");
  }
}

bool
BasePacketAssembler::getBuffer(const uchar *& buffer, size_t & size)
{
  bool result = currentSize_ > 0;
  buffer = currentBuffer_;
  currentBuffer_ = 0;
  size = currentSize_;
  currentSize_ = 0;
  return result;
}

