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
#include "FixedSizeHeaderAnalyzer.h"
#include <Common/Types.h>
#include <Codecs/DataSource.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FixedSizeHeaderAnalyzer::FixedSizeHeaderAnalyzer(
  size_t sizeBytes,
  bool bigEndian,
  size_t prefixBytes,
  size_t suffixBytes,
  size_t sequenceOffset,
  size_t sequenceLength
  )
: prefixBytes_(prefixBytes)
, sizeBytes_(sizeBytes)
, suffixBytes_(suffixBytes)
, sequenceOffset_(sequenceOffset)
, sequenceLength_(sequenceLength)
, bigEndian_(bigEndian)
, swapNeeded_(ByteSwapper::isBigEndian() ? !bigEndian : bigEndian)
, state_(ParsingIdle)
, blockSize_(0)
, byteCount_(0)
, testSkip_(0)
, headersParsed_(0)
{
}

FixedSizeHeaderAnalyzer::~FixedSizeHeaderAnalyzer()
{
}

bool
FixedSizeHeaderAnalyzer::analyzeHeader(DataSource & source, size_t & blockSize, bool & skip)
{
  skip = false;
  blockSize = 0;
  while(state_ != ParsingComplete)
  {
    switch(state_)
    {
    case ParsingIdle:
      {
        source.beginField("FIXED_SIZE_HEADER");
        state_ = ParsingPrefix;
        byteCount_ = 0;
        break;
      }
    case ParsingPrefix:
      {
        while(byteCount_ < prefixBytes_)
        {
          uchar next = 0;
          if(!source.getByte(next))
          {
            return false;
          }
          ++byteCount_;
        }
        state_ = ParsingBlockSize;
        byteCount_ = 0;
        blockSize_ = 0;
        break;
      }
    case ParsingBlockSize:
      {
        while(byteCount_ < sizeBytes_)
        {
          uchar next = 0;
          if(!source.getByte(next))
          {
            return false;
          }
          if(swapNeeded_)
          {
            blockSize |= (next & 0xFF) << (byteCount_ * 8);
          }
          else
          {
            blockSize_ <<= 8;
            blockSize_ |= (next & 0xFF);
          }
          ++byteCount_;
        }
        state_ = ParsingSuffix;
        byteCount_ = 0;

        break;
      }
    case ParsingSuffix:
      {
        while(byteCount_ < suffixBytes_)
        {
          uchar next = 0;
          if(!source.getByte(next))
          {
            return false;
          }
          ++byteCount_;
        }
        state_ = ParsingComplete;
        break;
      }
    default:
      {
        break;
      }
    }
  }
  state_ = ParsingIdle;
  blockSize = blockSize_;
  blockSize_ = 0;
  byteCount_ = 0;
  if(testSkip_ != 0 && (++headersParsed_ % testSkip_ == 0))
  {
    std::cout << std::endl << "SKIPPING HEADER " << headersParsed_ << std::endl;
    skip = true;
  }
  return true;
}

bool
FixedSizeHeaderAnalyzer::supportsSequenceNumber()const
{
  return true;
}

uint32
FixedSizeHeaderAnalyzer::getSequenceNumber(const uchar * buffer) const
{
  uint32 value = 0;
  if(bigEndian_)
  {
    size_t nByte = 0;
    while(nByte < sequenceLength_)
    {
      value <<= 8;
      value |= buffer[sequenceOffset_ + nByte++];
    }
  }
  else
  {
    size_t nByte = sequenceLength_;
    while(nByte != 0)
    {
      value <<= 8;
      value |= buffer[sequenceOffset_ + --nByte];
    }
  }
  return value;
}


void
FixedSizeHeaderAnalyzer::reset()
{
  state_ = ParsingIdle;
  blockSize_ = 0;
  byteCount_ = 0;
}
