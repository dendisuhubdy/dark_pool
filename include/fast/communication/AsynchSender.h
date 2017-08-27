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
#ifndef ASYNCHSENDER_H
#define ASYNCHSENDER_H
#include <Common/QuickFAST_Export.h>
#include "AsynchSender_fwd.h"
#include <Communication/Sender.h>
#include <Communication/AsioService.h>
#include <Common/Types.h>
namespace QuickFAST
{
  namespace Communication
  {
    /// @brief Base class for Sender that write the data asynchronously
    class QuickFAST_Export AsynchSender: public Sender
    {
    public:
      /// @brief Construct
      /// @param recycler will receive empty buffers after their contents have been written
      /// @param name a name for this connection
      AsynchSender(
        BufferRecycler & recycler,
        const char * name);

      /// @brief Construct
      /// @param ioService a shared io_service
      /// @param recycler will receive empty buffers after their contents have been written
      /// @param name a name for this connection
      AsynchSender(
        boost::asio::io_service & ioService,
        BufferRecycler & recycler,
        const char * name);

      virtual ~AsynchSender();

      // Override Sender method
      virtual void stop();

      // Override Sender method
      virtual void close();

      /// @brief Start the sender -- delegated to the io_service
      virtual void runThreads(size_t threadCount = 0, bool useThisThread = true)
      {
        ioService_.runThreads(threadCount, useThisThread);
      }

      /// @brief Wait for the dust to settle after stopping.
      virtual void joinThreads()
      {
        ioService_.joinThreads();
      }

      /// @brief Prepare to reuse this object -- consider creating a new one instead.
      virtual void resetService()
      {
        ioService_.resetService();
      }

      /// @brief Start the sender using only this thread -- delegated to the io_service
      virtual void run()
      {
        ioService_.run();
      }

      /// @brief Process one event in this thread -- delegated to the io_service
      virtual void run_one()
      {
        ioService_.run_one();
      }

      /// @brief Process any pending events in this thread -- delegated to the io_service
      virtual size_t poll()
      {
        return ioService_.poll();
      }

      /// @brief Process at most one pending event in this thread -- delegated to the io_service
      virtual size_t poll_one()
      {
        return ioService_.poll_one();
      }

      /// @brief a hack
      AsioService & peekAtIoServiceForDebugging()
      {
        return ioService_;
      };

    protected:
      /// @brief handle the write-complete event
      /// @param error contains the boost system error code (if any)
      /// @param buffer is the buffer that was written
      /// @param bytesWritten is the number of bytes written to the buffer
      void handleWrite(
        const boost::system::error_code& error,
        LinkedBuffer * buffer,
        size_t bytesWritten);

    protected:
      /// @brief an arbitray name for this sender
      std::string name_;
      /// @brief the ioService to use
      AsioService ioService_;
      /// @brief a special object to keep the ioService alive even when it has no work to do
      /// needed for output-type service which may have nothing to write at the moment, unlike
      /// input-type services which should always have an outstanding read or an active handler
      /// callback.
      boost::scoped_ptr<boost::asio::io_service::work> keepAlive_;
    };
  }
}
#endif // ASYNCHFILESENDER_H
