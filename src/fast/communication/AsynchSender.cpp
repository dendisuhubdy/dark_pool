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
#include "AsynchSender.h"
#include <Communication/LinkedBuffer.h>
#include <Common/Exceptions.h>
using namespace QuickFAST;
using namespace Communication;

AsynchSender::AsynchSender(
  BufferRecycler & recycler,
  const char * name)
    : Sender(recycler)
    , name_(name)
    , ioService_()
    , keepAlive_(new boost::asio::io_service::work(ioService_))
{
  //std::cout << "Asynch Sender {" << (void *)this << "} keeping ioService " << (void*) &ioService_ << " alive." << std::endl;
}

AsynchSender::AsynchSender(
  boost::asio::io_service & ioService,
  BufferRecycler & recycler,
  const char * name)
    : Sender(recycler)
    , name_(name)
    , ioService_(ioService)
    , keepAlive_(new boost::asio::io_service::work(ioService_))
{
//  std::cout << "Asynch Sender {" << (void *)this << "} keeping shared ioService " << (void*) &ioService_ << " alive." << std::endl;
}

AsynchSender::~AsynchSender()
{
//  std::cout << "Asynch Sender {" << (void *)this << "} exiting." << std::endl;
}

void
AsynchSender::stop()
{
//  std::cout << "Asynch Sender {" << (void *)this << "} releasing keep-alive on  ioService " << (void*) &ioService_ << std::endl;
  keepAlive_.reset();
}

void
AsynchSender::close()
{
  keepAlive_.reset();
}

void
AsynchSender::handleWrite(
  const boost::system::error_code& error,
  LinkedBuffer * buffer,
  size_t bytesWritten)
{
  size_t requested = buffer->used();
//        std::cout << "Wrote " << bytesWritten << " bytes from buffer " << (void *)(buffer->get()) << '[' << buffer->capacity() << ']' << std::endl;
  recycle(buffer);
  if(error)
  {
    std::stringstream msg;
    msg
      << "Error writing to " << name_
      << " " << error.message();
    throw CommunicationError(msg.str());
  }
  else if(bytesWritten != requested)
  {
    std::stringstream msg;
    msg
      << "Error writing to " << name_
      << ". Tried to write " << requested
      << " bytes.  Actually wrote " << bytesWritten << " bytes.";
    throw CommunicationError(msg.str());
  }
}
