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
#ifndef QUICKFASTLOGGER_H
#define QUICKFASTLOGGER_H
#include "Logger_fwd.h"
namespace QuickFAST{
  namespace Common{
    /// @brief interface to be implemented by a consumer of decoded messages.
    class Logger
    {
    public:
      /// The importance of being a log message.
      typedef unsigned short LogLevel;
      /// @brief an irrecoverable error
      static const unsigned short QF_LOG_FATAL = 0;
      /// @brief a serious error from which we _might_ be able to recover
      static const unsigned short QF_LOG_SERIOUS = 1;
      /// @brief a warning that will not prevent further decoding/encoding
      static const unsigned short QF_LOG_WARNING = 2;
      /// @brief general information
      static const unsigned short QF_LOG_INFO = 3;
      /// @brief wordy messages -- usually to support debugging
      static const unsigned short QF_LOG_VERBOSE = 4;

      /// @brief a virtual destructor for form's sake.
      /// No one should ever delete a pointer to a logger, but just in case:
      virtual ~Logger(){}

      /// @brief Does consumer wish to see logs with the given importance level.
      /// @param level is the importance level. low numbers are more important.
      virtual bool wantLog(LogLevel level) = 0;

      /// @brief report an "interesting" event
      /// @param level is the importance level. low numbers are more important.
      /// @param message describes the event
      /// @returns true if decoding should continue; false to stop decoding
      virtual bool logMessage(LogLevel level, const std::string & message) = 0;

      /// @brief Report an error during the decoding process
      ///
      /// The message consumer should return false unless a recovery mechanism
      /// exists to resynchronize decoding with the input stream.
      /// @param message describes the error
      /// @returns true if decoding should continue; false to stop decoding
      virtual bool reportDecodingError(const std::string & message) = 0;

      /// @brief Report a communication error
      ///
      /// Although a true return will attempt to continue, there is no guarantee
      /// that recovery is possible.
      /// @param message describes the error
      /// @returns true if decoding should continue; false to stop decoding
      virtual bool reportCommunicationError(const std::string & message) = 0;

    };
  }
}
#endif /* QUICKFASTLOGGER_H */
