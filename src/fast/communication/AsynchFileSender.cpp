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

#if defined(_WIN32) // Not available on non-Windows systems

#include "AsynchFileSender.h"
#include <Communication/LinkedBuffer.h>
#include <Common/Exceptions.h>
#include <Common/AtomicOps.h>
using namespace QuickFAST;
using namespace Communication;

AsynchFileSender::AsynchFileSender(
  BufferRecycler & recycler,
  const char * fileName,
  unsigned long additionalAttributes)
    : AsynchSender(recycler, fileName)
    , additionalAttributes_(additionalAttributes)
    , handle_(ioService_)
    , offset_(0LL)
{
}
AsynchFileSender::AsynchFileSender(
  boost::asio::io_service & ioService,
  BufferRecycler & recycler,
  const char * fileName,
  unsigned long additionalAttributes)
    : AsynchSender(ioService, recycler, fileName)
    , additionalAttributes_(additionalAttributes)
    , handle_(ioService_)
    , offset_(0LL)
{
}

AsynchFileSender::~AsynchFileSender()
{
  if(handle_.is_open())
  {
    handle_.cancel();
    handle_.close();
  }
}


void
AsynchFileSender::open()
{
  HANDLE handle = ::CreateFile(
    name_.c_str(),
    GENERIC_WRITE,
    0,    // not shared
    NULL, // no security attributes
    CREATE_ALWAYS,
    FILE_FLAG_OVERLAPPED |  FILE_FLAG_SEQUENTIAL_SCAN | additionalAttributes_,
    NULL);

  if(handle == INVALID_HANDLE_VALUE)
  {
    std::stringstream msg;
    msg << "Error opening file " << name_;
    throw CommunicationError(msg.str());
  }

  handle_.assign(handle);
}

void
AsynchFileSender::send(LinkedBuffer * buffer)
{
  size_t bytesToWrite = buffer->used();
  long pos = offset_;
  while(!CASLong(&offset_, pos, pos + bytesToWrite))
  {
    pos = offset_;
  }
  sendAt(buffer, (size_t)pos);
}

void
AsynchFileSender::sendAt(LinkedBuffer * buffer, size_t offset)
{
  size_t bytesToWrite = buffer->used();
  if(bytesToWrite > 0)
  {
#if 0
    std::cout << name_ << ": Write "
      << bytesToWrite << " bytes in buffer "
      << (void *) buffer->get()
      << ": " << std::string((const char *)buffer->get(), bytesToWrite > 20 ? 20:bytesToWrite)
      << std::endl << "..." << std::endl;
#endif
    boost::asio::async_write_at(
      handle_,
      (boost::uint64_t)offset,
      boost::asio::buffer(buffer->get(), bytesToWrite),
      boost::bind(&AsynchFileSender::handleWrite,
        this,
        boost::asio::placeholders::error,
        buffer,
        boost::asio::placeholders::bytes_transferred)
      );
  }
  else
  {
    recycle(buffer);
  }
}

void
AsynchFileSender::close()
{
  if(handle_.is_open())
  {
    handle_.cancel();
    handle_.close();
  }
  AsynchSender::close();
}
#endif // _WIN32
