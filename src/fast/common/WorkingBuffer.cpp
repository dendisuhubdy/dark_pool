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
#include "WorkingBuffer.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
static const size_t initialCapacity = 20;
WorkingBuffer::WorkingBuffer()
: reverse_(false)
, capacity_(initialCapacity)
, startPos_(0)
, endPos_(0)
, buffer_(new uchar[initialCapacity])
{
}

WorkingBuffer::WorkingBuffer(const WorkingBuffer & rhs)
: reverse_(rhs.reverse_)
, capacity_(rhs.capacity_)
, startPos_(rhs.startPos_)
, endPos_(rhs.endPos_)
, buffer_(new uchar[rhs.capacity_])
{
  memcpy(buffer_.get(), rhs.buffer_.get(), capacity_);
}


WorkingBuffer::~WorkingBuffer()
{
}

WorkingBuffer &
WorkingBuffer::operator =(const WorkingBuffer & rhs)
{
  WorkingBuffer temp(rhs);
  this->swap(temp);
  return *this;
}

namespace
{
  template<typename TYPE>
  void swap_i(TYPE lhs, TYPE rhs)
  {
    TYPE temp = lhs;
    lhs = rhs;
    rhs = temp;
  }
}

void
WorkingBuffer::swap(WorkingBuffer & rhs)
{
  swap_i(rhs.reverse_, reverse_);
  swap_i(rhs.capacity_, capacity_);
  swap_i(rhs.startPos_, startPos_);
  swap_i(rhs.endPos_, endPos_);
  rhs.buffer_.swap(buffer_);
}


void
WorkingBuffer::clear(bool reverse, size_t capacity)
{
  reverse_ = reverse;
  if(capacity > capacity_)
  {
    boost::scoped_array<uchar> newBuffer(new uchar[capacity]);
    buffer_.swap(newBuffer);
    capacity_ = capacity;
  }
  if(reverse_)
  {
    startPos_ = capacity_;
  }
  else
  {
    startPos_ = 0;
  }
  endPos_ = startPos_;
}

void
WorkingBuffer::pop_front()
{
  if(startPos_ < endPos_)
  {
    ++startPos_;
  }
  else
  {
    throw UsageError("Coding error", "WorkingBuffer: pop_front on empty WorkingBuffer.");
  }
}

void
WorkingBuffer::push(uchar byte)
{
  if(reverse_)
  {
    if(startPos_ == 0)
    {
      grow();
    }
    assert(startPos_ > 0);
    buffer_[--startPos_] = byte;
  }
  else
  {
    if(endPos_ >= capacity_)
    {
      grow();
    }
    assert(endPos_ < capacity_);
    buffer_[endPos_++] = byte;
  }
}

void
WorkingBuffer::grow()
{
  grow(capacity_ * 3 / 2); // todo: parameterize growth rate?
}

void
WorkingBuffer::grow(size_t newCapacity)
{
  size_t oldCapacity = capacity_;
  if(newCapacity == 0)
  {
    newCapacity = initialCapacity;
  }
  boost::scoped_array<uchar> newBuffer(new uchar[newCapacity]);
  size_t delta = 0;
  if(reverse_)
  {
    delta = newCapacity - oldCapacity;
  }
  std::memcpy(newBuffer.get() + delta, buffer_.get(), capacity_);
  buffer_.swap(newBuffer);
  startPos_ += delta;
  endPos_ += delta;
  capacity_ = newCapacity;
}


void
WorkingBuffer::append(const WorkingBuffer & rhs)
{
  size_t bytesToAppend = rhs.size();
  if(reverse_)
  {
    if(startPos_ < bytesToAppend)
    {
      grow(size() + bytesToAppend);
    }
    std::memcpy(buffer_.get() + startPos_ - bytesToAppend, rhs.buffer_.get() + rhs.startPos_, bytesToAppend);
    startPos_ -= bytesToAppend;
  }
  else
  {
    if(endPos_ + bytesToAppend > capacity_)
    {
      grow(size() + bytesToAppend);
    }
    std::memcpy(buffer_.get() + endPos_, rhs.buffer_.get() + rhs.startPos_, bytesToAppend);
    endPos_ += bytesToAppend;
  }
}

void
WorkingBuffer::toString(std::string & result) const
{
  result.clear();
  result.reserve(size());
  result.append(reinterpret_cast<const char *>(buffer_.get()) + startPos_, size());
}

void
WorkingBuffer::hexDisplay(std::ostream & out, size_t wrap) const
{
  const unsigned char * ptr = buffer_.get() + startPos_;
  size_t size = endPos_ - startPos_;
  out << std::hex << std::setfill('0');
  size_t pos = 0;
  while(pos < size)
  {
    out << std::setw(2) << (unsigned short)(ptr[pos]);
    ++pos;
    if(pos < size)
    {
      if(wrap != 0 && pos % wrap == 0)
      {
        out << std::endl;
      }
      else
      {
        out << ' ';
      }
    }
  }
  out << std::setfill(' ') << std::dec << std::endl;
}

bool
WorkingBuffer::operator ==(const WorkingBuffer & rhs) const
{
  if(size() != rhs.size())
  {
    return false;
  }
  return 0 == memcmp(buffer_.get(), rhs.buffer_.get(), size());
}
