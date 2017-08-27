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
#include "AsioService.h"
#include <Common/Logger.h>

using namespace QuickFAST;
using namespace Communication;

boost::asio::io_service AsioService::sharedIoService_;
AtomicCounter AsioService::sharedRunningThreadCount_;

AsioService::AsioService()
  : stopping_(false)
  , threadCount_(0)
  , threadCapacity_(0)
  , ioService_(sharedIoService_)
  , usingSharedService_(true)
  , logger_(0)
{
//  std::cout << "Create ASIO service(shared): " << (void *) this << std::endl;
}

AsioService::AsioService(boost::asio::io_service & ioService)
  : stopping_(false)
  , threadCount_(0)
  , threadCapacity_(0)
  , ioService_(ioService)
  , usingSharedService_(false)
  , logger_(0)
{
//  std::cout << "Create ASIO service(specific): " << (void *) this << " :: " << (void *) &ioService <<  std::endl;
}

AsioService::~AsioService()
{
}

void
AsioService::setLogger(Common::Logger & logger)
{
  logger_ = & logger;
}

void
AsioService::stopService()
{
  stopping_ = true;
  logger_ = 0;
//  std::cout << "AsioService stop.\n" << std::flush;
  ioService_.stop();
}

void
AsioService::joinThreads()
{
  while(threadCount_ > 0)
  {
     --threadCount_;
    threads_[threadCount_]->join();
    threads_[threadCount_].reset();
  }
}

void
AsioService::runThreads(size_t threadCount /*= 0*/, bool useThisThread /* = true*/)
{
  if(threadCount > threadCapacity_)
  {
    boost::scoped_array<ThreadPtr> newThreads(new ThreadPtr[threadCount]);
    for(size_t nThread = 0; nThread < threadCount_; ++nThread)
    {
      newThreads[nThread] = threads_[nThread];
    }
    threads_.swap(newThreads);
    threadCapacity_ = threadCount;
  }
  while(threadCount_ < threadCount)
  {
    threads_[threadCount_].reset(
      new boost::thread(boost::bind(&AsioService::run, this)));
    ++threadCount_;
  }
  if(useThisThread)
  {
    run();
    joinThreads();
  }
}

void
AsioService::run()
{
  long tc = 0;
  if(usingSharedService_)
  {
    tc = ++sharedRunningThreadCount_;
  }
  else
  {
    tc = ++runningThreadCount_;
  }
//  std::ostringstream msg;
//  msg << '{' << (void *) this << " :: " << (void *) &ioService_ << "} Starting AsioService thread #" << tc << std::endl;
//  std::cout << msg.str();

  size_t count = 1;
  while(! stopping_ && count != 0)
  {
    try
    {
      count = ioService_.run();
    }
    catch (const std::exception & ex)
    {
      if(logger_ != 0)
      {
        logger_->reportCommunicationError(ex.what());
      }
      else
      {
        std::cerr << ex.what() << std::endl;
      }
    }
  }
//  std::ostringstream msg2;
//  msg2 << '{' << (void *) this << " :: " << (void *) &ioService_ << "} Stopping AsioService thread #" << tc << std::endl;
//  std::cout << msg2.str();

  --runningThreadCount_;
}

