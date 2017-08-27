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

#pragma once

#include "HeaderAnalyzer_fwd.h"
#include <Codecs/DataSource_fwd.h>
#include <Common/Types.h>
namespace QuickFAST{
  namespace Codecs{
    /// An interface to be used to adapt to various styles of block or message header
    class HeaderAnalyzer
    {
    public:
      /// @brief Typical virtual destructor
      virtual ~HeaderAnalyzer()
      {
      }

      /// @brief Analyze the header.
      ///
      /// This method must read the entire header (if any) from the DataSource
      /// leaving it positioned to read the first byte of the actual FAST-encoded
      /// message.
      ///
      /// analyzeHeader should get header bytes by calling source.getByte().  This call
      /// may return false meaning no more bytes are present right now.   In that case
      /// analyzeHeader should save its state and return false -- expecting to be called
      /// again when additional data becomes available.
      ///
      /// If the header contains a block size, the blockSize parameter should be
      /// set to be the number of bytes in the block AFTER THIS HEADER.  Otherwise
      /// blockSize should be set to zero or left unchanged.
      ///
      /// Based on the contents of the header (or the phase of the moon) it may
      /// decide that it does not need the contents of this message and set the
      /// skip parameter accordingly.   The decoder can make of that what it will.
      ///
      /// In particular, even if analyzeHeader returns skip as true, the application may still
      /// see the decoded contents of the block due to restrictions in the protocol.
      ///
      /// @param[in] source for the bytes that make up the header.
      /// @param[out] blockSize (if any) from the header.  zero means unspecified.
      /// @param[out] skip true if this message should ignored if possible.
      /// @returns true if header is complete; false if more data is needed
      virtual bool analyzeHeader(DataSource & source, size_t & blockSize, bool & skip) = 0;

      /// @brief does this header analyzer support sequence numbers
      virtual bool supportsSequenceNumber()const
      {
        return false;
      }
      /// @brief Extract a sequence number from a header
      ///
      /// This message assumes the entire header is available in memory.
      /// This method does NOT consume or modify any data.  It may be called
      /// repeatedly and will produce the same result each time.
      /// @param buffer points to the header
      /// @returns the sequence number;
      virtual uint32 getSequenceNumber(const uchar * buffer) const
      {
        return 0;
      }

      /// @brief reset the header analyzer -- called when something went wrong while analyzing previous header
      virtual void reset()
      {
      }
    };
  }
}
#endif // HEADERANALYZER_H
