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
#ifndef ASYNCHFILESENDER_H
#define ASYNCHFILESENDER_H
#include <Common/QuickFAST_Export.h>
#include "AsynchFileSender_fwd.h"
#include <Communication/AsynchSender.h>
//#include <Common/Types.h>
namespace QuickFAST
{
  namespace Communication
  {
#if defined(_WIN32)
    /// @brief Sender that writes data to a fil asynchronously (WINDOWS ONLY)
    class QuickFAST_Export AsynchFileSender: public AsynchSender
    {
    public:
      /// @brief Construct
      /// @param recycler will receive empty buffers after their contents have been written
      /// @param fileName is the name of the file to be written
      /// @param additionalAttributes will be used in the Windows CreateFile call.
      AsynchFileSender(
        BufferRecycler & recycler,
        const char * fileName,
        unsigned long additionalAttributes = 0);

      /// @brief Construct
      /// @param ioService is a shared io_service.
      /// @param recycler will receive empty buffers after their contents have been written
      /// @param fileName is the name of the file to be written
      /// @param additionalAttributes will be used in the Windows CreateFile call.
      AsynchFileSender(
        boost::asio::io_service & ioService,
        BufferRecycler & recycler,
        const char * fileName,
        unsigned long additionalAttributes = 0);

      virtual ~AsynchFileSender();

      // Override Sender method
      virtual void open();
      // Override Sender method
      virtual void send(LinkedBuffer * buffer);

      /// @brief write a buffer to a specific location in the file
      ///
      /// relevant only to file I/O.  Does not make sense on other senders
      /// hence it's not virtual.
      ///
      /// Note that the send method delegates the write to this method after
      /// updating offset_.  This means the records will be written in the order
      /// the arrive at send, which is not necessarily the order in which they
      /// started in a multithreaded environment.  By allowing the caller to
      /// specify an offset, this method give the caller control over the order
      /// of data in the file.
      void sendAt(LinkedBuffer * buffer, size_t offset);

      // Override Sender method
      virtual void close();


    private:
      uint32 additionalAttributes_; // consider: FILE_FLAG_NO_BUFFERING
      boost::asio::windows::random_access_handle handle_;
      long volatile offset_;
    };
#endif // _WIN32
  }
}
#endif // ASYNCHFILESENDER_H
