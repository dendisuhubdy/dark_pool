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
#ifndef NOHEADERANALYZER_H
#define NOHEADERANALYZER_H
#include <Codecs/HeaderAnalyzer.h>
namespace QuickFAST{
  namespace Codecs{
    /// A Header Analyzer for messsages that have no headers.
    class NoHeaderAnalyzer
      : public HeaderAnalyzer
    {
    public:
      NoHeaderAnalyzer()
        : testSkip_(0)
        , headersParsed_(0)
      {
      }
      /// @brief Typical virtual destructor
      virtual ~NoHeaderAnalyzer()
      {
      }
      virtual bool analyzeHeader(
        DataSource & source,
        size_t & blockSize,
        bool & skip);

      /// @brief For debuging.  Force this to skip every "testSkip"th record.
      /// @param testSkip is how often to skip a record.  Zero means never skip.
      void setTestSkip(size_t testSkip)
      {
        testSkip_ = testSkip;
      }

    private:
      size_t testSkip_;
      size_t headersParsed_;
    };

    inline
    bool NoHeaderAnalyzer::analyzeHeader(
      DataSource & /*source*/,
      size_t & blockSize,
      bool & skip)
    {
      blockSize = 0;
      skip = false;
      if(testSkip_ != 0 && (++headersParsed_ % testSkip_ == 0))
      {
        std::cout << "SKIPPING HEADER " << headersParsed_ << std::endl;
        skip = true;
      }

      return true;
    }

  }
}
#endif // NOHEADERANALYZER_H
