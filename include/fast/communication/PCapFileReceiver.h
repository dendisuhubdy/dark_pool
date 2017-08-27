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
#ifndef PCAPFILERECEIVER_H
#define PCAPFILERECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "PCapFileReceiver_fwd.h"
#include <Communication/SynchReceiver.h>
#include <Communication/PCapReader.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// A Receiver that reads input from an istream.
    class PCapFileReceiver
      : public SynchReceiver
    {
    public:
      /// @brief Wrap a PCapFileReader into a Receiver
      ///
      /// @param filename names the PCap file
      /// @param wordSize is the word size of the platform where the file was captured.
      PCapFileReceiver(
        const std::string & filename,
        size_t wordSize = 0
        )
        : SynchReceiver()
        , filename_(filename)
        , wordSize_(wordSize)
      {
      }

      ~PCapFileReceiver()
      {
      }

    private:

      // Implement Receiver method
      virtual bool initializeReceiver()
      {
        if(wordSize_ == 32)
        {
          reader_.set32bit(true);
        }
        else if (wordSize_ == 64)
        {
          reader_.set64bit(true);
        }
        reader_.open(filename_.c_str()); // for debugging, dump to->, &std::cout);
        return reader_.good();
      }

      // Implement Receiver method
      bool fillBuffer(LinkedBuffer * buffer, boost::mutex::scoped_lock& lock)
      {
        const unsigned char * pcapBuffer;
        size_t pcapSize;
        bool result = reader_.read(pcapBuffer, pcapSize);
        if(result)
        {
          if(pcapSize <= buffer->capacity())
          {
            memcpy(buffer->get(), pcapBuffer, pcapSize);
            acceptFullBuffer(buffer, pcapSize, lock);
            result = true;
          }
        }
        return result;
      }

      // Implement Receiver method
      virtual void resetService()
      {
        return;
      }
    private:
      std::string filename_;
      size_t wordSize_;
      PCapReader reader_;
    };
  }
}
#endif // PCAPFILERECEIVER_H
