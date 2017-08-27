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

#ifndef RECOVERYFEED_H
#define RECOVERYFEED_H
#include "RecoveryFeed_fwd.h"
#include <Communication/LinkedBuffer.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// @brief An abstract base class for a source of packets used to recover during arbitrage.
    class RecoveryFeed
    {
    public:
      virtual ~RecoveryFeed() { }

      /// @brief Report that a gap in input sequence numbers has been detected.
      /// @returns true if the gap will be filled; false if the gap should be skipped
      virtual bool reportGap(sequence_t firstMissing, sequence_t firstPresentAfterGap) = 0;

      /// @brief A reminder from the Arbitrage process that a gap still exists.
      ///
      /// This gap has been reported previously via reportGap()
      /// The gap reported by this call may be smaller than what was previously reported
      /// if new packets have arrived, but it will never be larger.
      /// @returns true if the gap will be filled; false if the gap should be skipped
      virtual bool stillWaiting(sequence_t firstMissing, sequence_t firstPresentAfterGap) = 0;

      /// @brief Accept incoming packet
      void acceptBuffer(Communication::LinkedBuffer * buffer)
      {
        {
          boost::mutex::scoped_lock lock(inputMutex_);
          inputBuffers_.push(buffer);
        }
        inputWait_.notify_all();
      }

      /// @brief transfer all incoming packets into the supplied queue.
      ///
      /// @param queue to hold incoming packets
      void fetchBuffers(Communication::BufferQueue & queue)
      {
        boost::mutex::scoped_lock lock(inputMutex_);
        queue.push(inputBuffers_);
      }

      /// @brief Wait for a gap fill or a timeout.
      /// No guarantee that the gap will be filled.
      /// @param timeout is how long to wait.
      void waitGapFill(boost::posix_time::time_duration timeout)
      {
        boost::mutex::scoped_lock lock(inputMutex_);
        if(inputBuffers_.isEmpty())
        {
          boost::system_time until = boost::get_system_time();
          until += timeout;
          inputWait_.timed_wait(lock, until);
        }
      }

      /// @brief accept a buffer after its contents have been used.
      ///
      /// @param buffer is the now-empty buffer ready for re-use.
      void releaseBuffer(Communication::LinkedBuffer * buffer)
      {
        {
          boost::mutex::scoped_lock lock(freeMutex_);
          freeBuffers_.push(buffer);
        }
        freeWait_.notify_one();
      }

      /// @brief get an empty buffer into which data can be put.
      /// if no buffer is available, return 0
      Communication::LinkedBuffer * getFreeBuffer()
      {
        boost::mutex::scoped_lock lock(freeMutex_);
        return freeBuffers_.pop();
      }

      /// @brief wait for an empty buffer into which data can be put.
      /// if no buffer is available, wait "forever"
      Communication::LinkedBuffer * waitFreeBuffer()
      {
        boost::mutex::scoped_lock lock(freeMutex_);
        Communication::LinkedBuffer * buffer = freeBuffers_.pop();
        while(buffer == 0)
        {
          freeWait_.wait(lock);
          buffer = freeBuffers_.pop();
        }
        return buffer;
      }

    protected:
      /// Protects inputBuffers_
      boost::mutex inputMutex_;
      /// Waits for inputBuffers_
      boost::condition_variable inputWait_;
      /// Buffers ready to be delivered to Assembler
      Communication::BufferQueue inputBuffers_;
      /// Protects freeBuffers_
      boost::mutex freeMutex_;
      /// Waits for freeBuffers_
      boost::condition_variable freeWait_;
      /// Empty buffers returned from Assembler
      Communication::BufferQueue freeBuffers_;
    };
  }
}

#endif // CUSTOMFEED_FWD_H
