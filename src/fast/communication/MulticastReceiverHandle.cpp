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
#include "MulticastReceiverHandle.h"
#include <Communication/MulticastReceiver.h>

namespace QuickFAST{
  namespace Communication {
    class MulticastReceiverHandle_i
    {
    public:
      MulticastReceiverHandle_i(
        const std::string & multicastGroupIP,
        const std::string & listenInterfaceIP,
        const std::string & bindIP,
        unsigned short portNumber)
        :ptr_(new MulticastReceiver(
                  multicastGroupIP,
                  listenInterfaceIP,
                  bindIP,
                  portNumber))
      {
      }

      MulticastReceiverPtr ptr_;
    };
  }
}


using namespace QuickFAST;
using namespace Communication;

MulticastReceiverHandle::MulticastReceiverHandle(
    const std::string & multicastGroupIP,
    const std::string & listenInterfaceIP,
    const std::string & bindIP,
    unsigned short portNumber)
  : pImpl_(new MulticastReceiverHandle_i(
            multicastGroupIP,
            listenInterfaceIP,
            bindIP,
            portNumber))
{
}

MulticastReceiverHandle::~MulticastReceiverHandle()
{
  delete pImpl_;
}

size_t
MulticastReceiverHandle::bytesReadable() const
{
  return pImpl_->ptr_->bytesReadable();
}

size_t
MulticastReceiverHandle::noBufferAvailable() const
{
  return pImpl_->ptr_->noBufferAvailable();
}

size_t
MulticastReceiverHandle::packetsReceived() const
{
  return pImpl_->ptr_->packetsReceived();
}

size_t
MulticastReceiverHandle::packetsQueued() const
{
  return pImpl_->ptr_->packetsQueued();
}

size_t
MulticastReceiverHandle::batchesProcessed() const
{
  return pImpl_->ptr_->batchesProcessed();
}

size_t
MulticastReceiverHandle::packetsProcessed() const
{
  return pImpl_->ptr_->packetsProcessed();
}

size_t
MulticastReceiverHandle::packetsWithErrors() const
{
  return pImpl_->ptr_->packetsWithErrors();
}

size_t
MulticastReceiverHandle::emptyPackets() const
{
  return pImpl_->ptr_->emptyPackets();
}

size_t
MulticastReceiverHandle::bytesReceived() const
{
  return pImpl_->ptr_->bytesReceived();
}

size_t
MulticastReceiverHandle::bytesProcessed() const
{
  return pImpl_->ptr_->bytesProcessed();
}

size_t
MulticastReceiverHandle::largestPacket() const
{
  return pImpl_->ptr_->largestPacket();
}


void
MulticastReceiverHandle::start(
  Communication::Assembler & assembler,
  size_t bufferSize,
  size_t bufferCount)
{
  pImpl_->ptr_->start(assembler, bufferSize, bufferCount);
}

void
MulticastReceiverHandle::run(unsigned short additionalThreads, bool useThisThread)
{
  pImpl_->ptr_->runThreads(additionalThreads, useThisThread);
}

void
MulticastReceiverHandle::joinThreads()
{
  pImpl_->ptr_->joinThreads();
}

void
MulticastReceiverHandle::stop()
{
  pImpl_->ptr_->stop();
}
