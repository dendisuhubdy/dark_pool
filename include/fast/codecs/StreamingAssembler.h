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
#ifndef STREAMINGASSEMBLER_H
#define STREAMINGASSEMBLER_H

#include <Common/QuickFAST_Export.h>

#include <Communication/Assembler.h>
#include <Communication/LinkedBuffer.h>
#include <Codecs/DataSource.h>
#include <Codecs/HeaderAnalyzer.h>
#include <Codecs/Decoder.h>
#include <Codecs/TemplateRegistry_fwd.h>
#include <Messages/ValueMessageBuilder_fwd.h>

namespace QuickFAST
{
  namespace Codecs
  {
    /// @brief Service a Receiver's Queue when expecting streaming data (TCP/IP) with (or without) block headers.
    class QuickFAST_Export StreamingAssembler
      : public Communication::Assembler
      , public Codecs::DataSource
    {
    public:
      /// @brief Constuct the Assembler
      /// @param templateRegistry defines the decoding instructions for the decoder
      /// @param headerAnalyzer analyzes the header of each message (if any)
      /// @param builder receives the data from the decoder.
      /// @param waitForCompleteMessage if true cause decoding to be delayed (without thread blocking)
      ///        until a complete message is available.
      StreamingAssembler(
          TemplateRegistryPtr templateRegistry,
          HeaderAnalyzer & headerAnalyzer,
          Messages::ValueMessageBuilder & builder,
          bool waitForCompleteMessage = false);

      virtual ~StreamingAssembler();

      /// @brief set the maximum number of messages to decode
      /// @param messageLimit is the number of messages to decode
      void setMessageLimit(size_t messageLimit)
      {
        messageLimit_ = messageLimit;
      }

      ///////////////////////////
      // Implement Assembler
      virtual void receiverStarted(Communication::Receiver & receiver);
      virtual void receiverStopped(Communication::Receiver & receiver);
      virtual bool serviceQueue(Communication::Receiver & receiver);

      ///////////////////////
      // Implement DataSource
      //
      virtual bool getBuffer(const uchar *& buffer, size_t & size);
      virtual int messageAvailable();

      /// @brief Access the internal decoder
      /// @returns a reference to the internal decoder
      Codecs::Decoder & decoder()
      {
        return decoder_;
      }

    private:
      StreamingAssembler & operator = (const StreamingAssembler &);
      StreamingAssembler(const StreamingAssembler &);
      StreamingAssembler();

    private:
      HeaderAnalyzer & headerAnalyzer_;
      Messages::ValueMessageBuilder & builder_;
      bool stopping_;
      bool waitForCompleteMessage_;

      // Nonzero during call to consumeBuffer ->decoder
      // zero the rest of the time.
      Communication::Receiver * receiver_;

      // buffer from which data is being pulled
      Communication::LinkedBuffer * currentBuffer_;

      bool headerIsComplete_;
      bool skipBlock_;
      size_t blockSize_;

      bool inDecoder_;

      size_t messageCount_;
      size_t byteCount_;
      size_t messageLimit_;
    };
  }
}
#endif // STREAMINGASSEMBLER_H
