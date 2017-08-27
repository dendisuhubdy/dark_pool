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
#include "DataSource.h"
#include <Common/Exceptions.h>
using namespace QuickFAST;
using namespace Codecs;

DataSource::DataSource()
: buffer_(0)
, size_(0)
, position_(0)
, echo_(0)
, raw_(false)
, hex_(true)
, verboseMessages_(true)
, verboseFields_(false)
, byteCount_(0)
, isPmap_(false)
{
}

DataSource::~DataSource()
{
}

int
DataSource::messageAvailable()
{
  // the default implementation assumes that any data at all means the next message will
  // appear when needed.
  return bytesAvailable();
}

int
DataSource::bytesAvailable()
{
  if(position_ >= size_)
  {
    position_ = 0;
    size_ = 0;
    (void)getBuffer(buffer_, size_);
  }
  return int(size_ - position_);
}

void
DataSource::doEcho(bool ok, uchar byte)
{
  if(ok)
  {
    ++byteCount_;
    if(hex_)
    {
      (*echo_) << std::hex << std::setw(2) << std::setfill('0')
        << short(byte) << ' '
        << std::setfill(' ') << std::setw(0) << std::dec;
    }
    else if(raw_)
    {
      echo_->put(byte);
    }
    if(verboseFields_)
    {
      echoString_ += byte;
    }
  }
  else
  {
    if(verboseMessages_)
    {
      (*echo_) << "*** End of data @" << std::hex << byteCount_ << std::dec << "***" << std::endl;
    }
  }
}

void
DataSource::setEcho(
  std::ostream & echo,
  const EchoType& echoType /* = HEX */,
  bool verboseMessages/* = true */,
  bool verboseFields/* = false */)
{
  echo_ = &echo;
  if (echoType == HEX)
  {
    raw_ = false;
    hex_ = true;
  }
  else if (echoType == RAW)
  {
    raw_ = true;
    hex_ = false;
  }
  else if (echoType == NONE)
  {
    raw_ = false;
    hex_ = false;
  }
  verboseMessages_ = verboseMessages;
  verboseFields_ = verboseFields;
}

void
DataSource::beginMessage()
{
  if(echo_ && verboseMessages_)
  {
    (*echo_) << std::endl << "***MESSAGE @" << std::hex << byteCount_ << std::dec << "***" << std::endl;
  }
}

void
DataSource::beginField(const std::string & name)
{
  if(echo_ && verboseFields_)
  {
    if(!echoString_.empty())
    {
      if(isPmap_)
      {
        (*echo_) << std::endl << "  PMAP: ";
        for(size_t pos = 0; pos < echoString_.length(); ++pos)
        {
          unsigned char byte = echoString_[pos];
          for(size_t b = 0; b < 7; ++b)
          {
            (*echo_) << ' ' << (short)(byte >>6 & 1);
            byte <<= 1;
          }
        }
      }
      else
      {
        (*echo_) << std::endl << "  Value: \"";
        unsigned long long unsignedInt = 0;
        long long signedInt = 0;
        for(size_t pos = 0; pos < echoString_.length(); ++pos)
        {
          unsigned char byte = echoString_[pos];
          char c = (byte & 0x7f);
          if(c < ' ' || c == 0x7F) c = '.';
          (*echo_) << c;
          if(pos == 0 && (c & 0x40) != 0)
          {
            signedInt = -1;
          }
          signedInt <<= 7;
          signedInt += (byte & 0x7F);
          unsignedInt <<= 7;
          unsignedInt += (byte & 0x7F);
        }
        (*echo_) << "\": " << signedInt;
        if(signedInt < 0)
        {
          (*echo_) << " (" << unsignedInt << ')';
        }
      }
      echoString_.erase();
    }
    isPmap_ = name == "PMAP";
    (*echo_) << std::endl << "***Field: " << name << " @" << std::hex<< byteCount_ << std::dec << "***" << std::endl << "  ";
  }
}
