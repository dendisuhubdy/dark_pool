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
#ifndef FASTENCODEDHEADERANALYZER_H
#define FASTENCODEDHEADERANALYZER_H
#include <Common/QuickFAST_Export.h>
#include <Codecs/HeaderAnalyzer.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief Handle a FAST encoded block header
    class QuickFAST_Export FastEncodedHeaderAnalyzer
      : public HeaderAnalyzer
    {
    public:

      /// @brief Construct
      ///
      /// Ignore: prefix FAST fields (looking for stop bits)
      /// Interpret: the next FAST field as block size if hasBlockSize is true
      /// Ignore: suffix FAST fields (looking for stop bits)
      ///
      /// @param prefix is the number of fields before the block size
      /// @param suffix is the number of fields after the block size
      /// @param hasBlockSize is true if the blocksize is present in the header
      explicit FastEncodedHeaderAnalyzer(
        size_t prefix = 0,
        size_t suffix = 0,
        bool hasBlockSize = false
        );

      /// @brief Typical virtual destructor
      virtual ~FastEncodedHeaderAnalyzer();

      ////////////////////////
      // Implement HeaderAnalyzer
      virtual bool analyzeHeader(DataSource & source, size_t & blockSize, bool & skip);
    private:
      size_t prefixCount_;
      size_t suffixCount_;
      enum
      {
        ParsingIdle,
        ParsingPrefix,
        ParsingBlockSize,
        ParsingSuffix,
        ParsingComplete
      } state_;
      bool hasBlockSize_;
      size_t blockSize_;
      size_t fieldCount_;
    };
  }
}
#endif // FASTENCODEDHEADERANALYZER_H
