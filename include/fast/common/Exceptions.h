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
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

//#include <Common/QuickFAST_Export.h>
namespace QuickFAST{
  /// @brief Exception to be thrown when an attempt is made to convert a value to an incompatable type.
  class /* QuickFAST_Export */ UnsupportedConversion : public std::domain_error{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    UnsupportedConversion(const std::string & reason)
      : std::domain_error(reason.c_str())
    {
    }

    /// @brief Special construtor to provide explicit type information about the failed conversion.
    ///
    /// @param actualType is the type of actually found
    /// @param desiredType is the type that was requested
    UnsupportedConversion(const std::string & actualType, const std::string & desiredType)
      : std::domain_error((actualType + " cannot be converted to " + desiredType).c_str())
    {
    }
  };

  /// @brief Exception to be thrown when a value does not fit in a field
  class /* QuickFAST_Export */ OverflowError : public std::overflow_error{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    OverflowError(const std::string & reason)
      : std::overflow_error(reason.c_str())
    {
    }
  };

  /// @brief Exception to be thrown when a mandatory field is not present or a requested field is not found.
  class /* QuickFAST_Export */ FieldNotPresent : public std::invalid_argument{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    FieldNotPresent(const std::string & reason)
      : std::invalid_argument(reason.c_str())
    {
    }
  };

  /// @brief Exception to be thrown when a dynamic error is detected while encoding or decoding
  class /* QuickFAST_Export */ EncodingError : public std::runtime_error{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    EncodingError(const std::string & reason)
      : std::runtime_error(reason.c_str())
    {
    }
  };

  /// @brief Exception to be thrown when a static error is detected while parsing or using templates.
  class /* QuickFAST_Export */ TemplateDefinitionError: public std::runtime_error{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    TemplateDefinitionError(const std::string & reason)
      : std::runtime_error(reason.c_str())
    {
    }
  };

  /// @brief Exception to be thrown when processing cannot continue due to a communication error
  class /* QuickFAST_Export */ CommunicationError: public std::runtime_error{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param reason describes the problem.
    CommunicationError(const std::string & reason)
      : std::runtime_error(reason.c_str())
    {
    }
  };

  /// @brief Exception to be thrown when the application misuses QuickFAST
  class /* QuickFAST_Export */ UsageError : public std::invalid_argument{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param errorType the type of problem. (Coding Error, Internal Error, etc.)
    /// @param description the actual problem.
    UsageError(const char * errorType, const char * description)
      : std::invalid_argument((std::string(errorType) + ": " +  description).c_str())
    {
    }
  };

  /// @brief Exception to be thrown when an internal logic error is detected
  class /* QuickFAST_Export */ InternalError : public std::logic_error{
  public:
    /// @brief Construct explaining what's wrong.
    /// @param errorType the type of problem. (Coding Error, Internal Error, etc.)
    /// @param description the actual problem.
    InternalError(const char * errorType, const char * description)
      : std::logic_error((std::string(errorType) +  ": " + description).c_str())
    {
    }
  };
}
#endif // EXCEPTIONS_H
