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
#ifndef DATASOURCEBUFFER_H
#define DATASOURCEBUFFER_H
#include <Common/QuickFAST_Export.h>
#include <Codecs/DataSource.h>
namespace QuickFAST{
  namespace Codecs{
    /// A data source that uses an existing buffer.
    class QuickFAST_Export DataSourceBuffer : public DataSource
    {
    public:
      /// @brief Deliver data from an existing buffer.
      ///
      /// Does not take ownership or otherwise manipulate the buffer.
      /// @param buffer contains the data
      /// @param length is the number of bytes in the data
      explicit DataSourceBuffer(const unsigned char * buffer, size_t length);

      /// @brief a typical virtual destructor.
      virtual ~DataSourceBuffer();

      virtual bool getBuffer(const uchar *& buffer, size_t & size);

    private:
      const unsigned char * buffer_;
      size_t end_;
      bool first_;
    };
  }
}
#endif // DATASOURCEBUFFER_H
