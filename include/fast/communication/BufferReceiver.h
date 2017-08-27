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

#ifdef _MSC_VER
# pragma once
#endif
#ifndef BUFFERRECEIVER_H
#define BUFFERRECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "BufferReceiver_fwd.h"
#include <Communication/SynchReceiver.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// A Receiver that reads input from a memory buffer
    class BufferReceiver
      : public SynchReceiver
    {
    public:
      /// @brief A receiver that can accept data buffered in memory.
      ///
      BufferReceiver()
        : buffer_(0)
        , used_(0)
      {
      }

      ~BufferReceiver()
      {
      }

      virtual void receiveBuffer(
        const unsigned char * buffer,
        size_t used
        )
      {
        buffer_ = buffer;
        used_ = used;
        stopping_ = false;
        startReceiveUnlocked();
        while(!stopping_)
        {
          tryServiceQueue();
        }
      }

    private:

      // Implement Receiver method
      virtual bool initializeReceiver()
      {
        return true;
      }

      // Implement Receiver method
      bool fillBuffer(LinkedBuffer * buffer, boost::mutex::scoped_lock& lock)
      {
        bool result = false;
        if(used_ > 0)
        {
          size_t bytes = used_;
          used_ = 0;
          buffer->setExternal(buffer_, bytes);
          acceptFullBuffer(buffer, bytes, lock);
          result = true;
        }
        return result;
      }

      // Implement Receiver method
      virtual void resetService()
      {
        return;
      }
    private:
      const unsigned char * buffer_;
      size_t used_;
    };
  }
}
#endif // BUFFERRECEIVER_H
