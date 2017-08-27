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
#ifndef RAWFILERECEIVER_H
#define RAWFILERECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "RawFileReceiver_fwd.h"
#include <Communication/SynchReceiver.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// A Receiver that reads input from an istream.
    class RawFileReceiver
      : public SynchReceiver
    {
    public:
      /// @brief Wrap a standard istream into a Receiver
      ///
      /// The input stream should be opened in binary mode
      /// if that option is available/needed on your operating
      /// system and stream type. (i.e. specify std::ios::binary
      /// when you open a ofstream on Windows.)
      /// @param stream supplies the data
      RawFileReceiver(
        std::istream & stream
        )
        : stream_(stream)
      {
      }

      ~RawFileReceiver()
      {
      }

    private:

      // Implement Receiver method
      virtual bool initializeReceiver()
      {
        return stream_.good() && !stream_.eof();
      }

      // Implement Receiver method
      bool fillBuffer(LinkedBuffer * buffer, boost::mutex::scoped_lock& lock)
      {
        bool filling = false;
        if(!stopping_ && stream_.good() && !stream_.eof())
        {
          stream_.read(reinterpret_cast<char *>(buffer->get()), buffer->capacity());
          size_t size = (size_t) stream_.gcount();
          if(acceptFullBuffer(buffer, size, lock))
          {
            needService_ = true;
          }
          filling = true;
        }
        return filling;
      }

      // Implement Receiver method
      virtual void resetService()
      {
        return;
      }

    private:
      std::istream & stream_;
      bool needService_;
    };
  }
}
#endif // RAWFILERECEIVER_H
