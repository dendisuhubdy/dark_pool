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
#include "DataSourceStream.h"
using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

DataSourceStream::DataSourceStream(std::istream & stream, size_t bufferSize)
: stream_(stream)
, pos_(0)
, end_(0)
, capacity_(bufferSize)
{
  stream.seekg(0,std::ios::end);
  end_ = (size_t)stream.tellg();
  stream.seekg(0,std::ios::beg);
  if(capacity_ > end_)
  {
    capacity_ = end_;
  }
  buffer_.reset(new unsigned char[capacity_]);
}

DataSourceStream::~DataSourceStream()
{
}

bool
DataSourceStream::getBuffer(const uchar *& buffer, size_t & size)
{
  size = 0;
  if(stream_.good() && !stream_.eof())
  {
    buffer = buffer_.get();
    stream_.read(reinterpret_cast<char *>(buffer_.get()), capacity_);
    size = (size_t)stream_.gcount();
    pos_ += size;
  }
  return size > 0;
}
