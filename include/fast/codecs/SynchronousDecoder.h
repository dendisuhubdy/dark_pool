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
#ifndef SYNCHRONOUSDECODER_H
#define SYNCHRONOUSDECODER_H
#ifndef QUICKFAST_HEADERS
#error Please include <Application/QuickFAST.h> preferably as a precompiled header file.
#endif //QUICKFAST_HEADERS

#include <Codecs/TemplateRegistry.h>
#include <Codecs/Decoder.h>
#include <Codecs/DataSource.h>
#include <Messages/ValueMessageBuilder.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief Support Synchronous (blocking) decoding of a FAST data stream
    class SynchronousDecoder
    {
    public:
      /// @brief Construct given a template registry
      /// @param templateRegistry contains the templates to be used during decoding.
      explicit SynchronousDecoder(TemplateRegistryPtr templateRegistry)
      : decoder_(templateRegistry)
      , resetOnMessage_(false)
      , messageCount_(0)
      , messageCountLimit_(0)
      , maxFieldCount_(templateRegistry->maxFieldCount())
      , headerBytes_(0)
      {
      }

      ~SynchronousDecoder()
      {
      }

      /// @brief Set the flag to reset the decoder for every message
      ///
      /// Some data streams reset the Xcoder context at the beginning of each message
      /// This is particulary true when the data stream uses a datagram-per-message
      /// approach (UDP and Unreliable Multicast).
      /// @param reset true if the decoder should be reset for each message; default false
      void setResetOnMessage(bool reset)
      {
        resetOnMessage_ = reset;
      }

      /// @brief immediate reset
      void reset()
      {
        decoder_.reset();
      }

      /// @brief Enable some debugging/diagnostic information to be written to an ostream
      /// @param out the ostream to receive the data
      void setVerboseOutput(std::ostream & out)
      {
        decoder_.setVerboseOutput(out);
      }

      /// @brief Clear the verbose output that was enabled by setVerboseOutput()
      void disableVerboseOutput()
      {
        decoder_.disableVerboseOutput();
      }

      /// @brief Enable/disable strict checking of conformance to the FAST standard
      ///
      /// If strict is false some conditions such as integer overflow or inefficient encoding
      /// will be ignored.  The default is true -- strict checking is enabled.
      /// @param strict true to enable; false to disable strict checking
      void setStrict(bool strict)
      {
        decoder_.setStrict(strict);
      }

      /// @brief get the current status of the strict property.
      /// @returns true if strict checking is enabled.
      bool getStrict()const
      {
        return decoder_.getStrict();
      }

      /// @brief Get the id of the template driving the decoding
      template_id_t getTemplateId()const
      {
        return decoder_.getTemplateId();
      }

      /// @brief set an upper limit on the # of messages to be decoded
      /// @param messageCountLimit how many messages to decode (0 means "forever")
      void setLimit(size_t messageCountLimit)
      {
        messageCountLimit_ = messageCountLimit;
      }
      /// @brief set the number of bytes to skip at the beginning of each message
      /// @param headerBytes is the number of bytes to skip
      void setHeaderBytes(size_t headerBytes)
      {
        headerBytes_ = headerBytes;
      }

      /// @brief How many messages have been decoded.
      /// @returns the number of messages that have been decoded.
      size_t messageCount() const
      {
        return messageCount_;
      }

      /// @brief Run the decoding process
      ///
      /// Runs until the DataSource reports end of data
      /// or the consumer returns false,
      /// or messageCount() messages have been decoded.
      void decode(
        DataSource & source,
        Messages::ValueMessageBuilder & builder)
      {
        while(source.messageAvailable() > 0 && (messageCountLimit_ == 0 || messageCount_ < messageCountLimit_))
        {
          if(resetOnMessage_)
          {
            decoder_.reset();
          }
//          if(headerBytes_ > 0)
//          {
//            std::cout << "Skipping [";
            for(size_t nHeadByte = 0; nHeadByte < headerBytes_; ++nHeadByte)
            {
              uchar byte = 0;
              if(!source.getByte(byte))
              {
                return;
              }
//              std::cout << std::hex << std::setw(2) << (unsigned short)byte << std::dec << ' ';
            }
//            std::cout << ']' << std::endl;
//          }
          decoder_.decodeMessage(source, builder);
          messageCount_ += 1;
        }
      }

    private:
      SynchronousDecoder();
      SynchronousDecoder(const SynchronousDecoder &);
      SynchronousDecoder & operator =(const SynchronousDecoder &);
    private:
      Decoder decoder_;
      bool resetOnMessage_;
      size_t messageCount_;
      size_t messageCountLimit_;
      size_t maxFieldCount_;
      size_t headerBytes_;
    };
  }
}

#endif /* SYNCHRONOUSDECODER_H */
