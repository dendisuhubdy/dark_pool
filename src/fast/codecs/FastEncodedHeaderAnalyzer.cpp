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
#include "FastEncodedHeaderAnalyzer.h"
#include <Common/Types.h>
#include <Codecs/DataSource.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FastEncodedHeaderAnalyzer::FastEncodedHeaderAnalyzer(
  size_t prefix,
  size_t suffix,
  bool hasBlockSize
  )
: prefixCount_(prefix)
, suffixCount_(suffix)
, state_(ParsingIdle)
, hasBlockSize_(hasBlockSize)
, blockSize_(0)
, fieldCount_(0)
{
}



FastEncodedHeaderAnalyzer::~FastEncodedHeaderAnalyzer()
{
}

bool
FastEncodedHeaderAnalyzer::analyzeHeader(DataSource & source, size_t & blockSize, bool & skip)
{
  while(state_ != ParsingComplete)
  {
    switch(state_)
    {
    case ParsingIdle:
      {
        state_ = ParsingPrefix;
        fieldCount_ = 0;
//        break;
      }
    case ParsingPrefix:
      {
        while(fieldCount_ < prefixCount_)
        {
          uchar next = 0;
          if(!source.getByte(next))
          {
            return false;
          }
          if((next & 0x80) != 0)
          {
            ++fieldCount_;
          }
        }
        state_ = ParsingBlockSize;
        blockSize_ = 0;
//        break;
      }
    case ParsingBlockSize:
      {
        if(!hasBlockSize_)
        {
          state_ = ParsingSuffix;
          fieldCount_ = 0;
        }
        while(state_ == ParsingBlockSize)
        {
          uchar next = 0;
          if(!source.getByte(next))
          {
            return false;
          }
          blockSize_ <<= 7;
          blockSize_ |= (next & 0x7f);
          if((next & 0x80) != 0)
          {
            state_ = ParsingSuffix;
            fieldCount_ = 0;
          }
        }
//        break;
      }
    case ParsingSuffix:
      {
        while(fieldCount_ < suffixCount_)
        {
          uchar next = 0;
          if(!source.getByte(next))
          {
            return false;
          }
          if((next & 0x80) != 0)
          {
            ++fieldCount_;
          }
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
  blockSize = blockSize_;
  skip = false;
  state_ = ParsingIdle;
  return true;
}
