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
#ifndef FIXEDSIZEHEADERANALYZER_H
#define FIXEDSIZEHEADERANALYZER_H
#include <Common/QuickFAST_Export.h>
#include "HeaderAnalyzer.h"
#include <Common/ByteSwapper.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief Handle a fixed Size block header
    class QuickFAST_Export FixedSizeHeaderAnalyzer
      : public HeaderAnalyzer
    {
    public:
      /// @brief Construct
      ///
      /// Ignore: prefixBytes
      /// Interpret: sizeBytes as a "bigEndian" number
      /// Ignore:suffixBytes
      ///
      /// Note sequence # support was added after-the-fact.  It clashes a bit with
      /// block size support, but they _can_ be used together if you are careful.
      ///
      /// @param sizeBytes is the number of bytes in the block size field.  Zero means no block size.
      /// @param bigEndian if true means the data on the wire is big endian (network order)
      /// @param prefixBytes is the number if bytes in the header before the block size.
      /// @param suffixBytes is the number if bytes in the header after the block size.
      /// @param sequenceOffset is the position of the sequenceNumber field in the header
      /// @param sequenceLength is the number of bytes in the sequenceNumber field
      explicit FixedSizeHeaderAnalyzer(
        size_t sizeBytes,
        bool bigEndian = false,
        size_t prefixBytes = 0,
        size_t suffixBytes = 0,
        size_t sequenceOffset = 0,
        size_t sequenceLength = 4
        );

      /// @brief Typical virtual destructor
      virtual ~FixedSizeHeaderAnalyzer();

      /// @brief For debuging.  Force this to skip every "testSkip"th record.
      /// @param testSkip is how often to skip a record.  Zero means never skip.
      void setTestSkip(size_t testSkip)
      {
        testSkip_ = testSkip;
      }

      ////////////////////////
      // Implement HeaderAnalyzer
      virtual bool analyzeHeader(DataSource & source, size_t & blockSize, bool & skip);
      virtual bool supportsSequenceNumber()const;
      virtual uint32 getSequenceNumber(const uchar * buffer) const;
      virtual void reset();
    private:
      size_t prefixBytes_;
      size_t sizeBytes_;
      size_t suffixBytes_;
      size_t sequenceOffset_;
      size_t sequenceLength_;
      bool bigEndian_;
      bool swapNeeded_;

      enum
      {
        ParsingIdle,
        ParsingPrefix,
        ParsingBlockSize,
        ParsingSuffix,
        ParsingComplete
      } state_;
      size_t blockSize_;
      size_t byteCount_;

      size_t testSkip_;
      size_t headersParsed_;
    };
  }
}
#endif // FIXEDSIZEHEADERANALYZER_H
