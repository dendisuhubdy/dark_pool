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

#ifndef BASEPACKETASSEMBLER_H
#define BASEPACKETASSEMBLER_H

#include "BasePacketAssembler_fwd.h"
#include <Common/QuickFAST_Export.h>

#include <Communication/Assembler.h>
#include <Codecs/Decoder.h>
#include <Codecs/DataSource.h>
#include <Codecs/HeaderAnalyzer.h>
#include <Codecs/TemplateRegistry_fwd.h>
#include <Messages/ValueMessageBuilder_fwd.h>

namespace QuickFAST
{
  namespace Codecs
  {
    /// @brief Service a Receiver's Queue when expecting packet boundaries to match message boundaries (UDP or Multicast)
    /// with (or without) block headers.
    class QuickFAST_Export BasePacketAssembler
      : public Communication::Assembler
      , public DataSource
    {
    public:
      /// @brief Constuct the Assembler
      /// @param templateRegistry defines the decoding instructions for the decoder
      /// @param packetHeaderAnalyzer analyzes the header of each packet (if any)
      /// @param messageHeaderAnalyzer analyzes the header of each message (if any)
      /// @param builder receives the data from the decoder.
      BasePacketAssembler(
          TemplateRegistryPtr templateRegistry,
          HeaderAnalyzer & packetHeaderAnalyzer,
          HeaderAnalyzer & messageHeaderAnalyzer,
          Messages::ValueMessageBuilder & builder);

      virtual ~BasePacketAssembler();

      /// @brief set the maximum number of messages to decode
      /// @param messageLimit is the number of messages to decode
      void setMessageLimit(size_t messageLimit)
      {
        messageLimit_ = messageLimit;
      }

      /// @brief Access the internal decoder
      /// @returns a reference to the internal decoder
      Codecs::Decoder & decoder()
      {
        return decoder_;
      }

      /// @brief Access the internal decoder (const)
      /// @returns a const reference to the internal decoder
      const Codecs::Decoder & decoder() const
      {
        return decoder_;
      }

      /// @brief How many messages have been processed
      /// @returns the message count
      size_t messageCount()const
      {
        return messageCount_;
      }

      /// @brief How many bytes have been processed
      /// @returns the byte count
      size_t byteCount()const
      {
        return byteCount_;
      }

      ///////////////////////////
      // Implement Assembler
      virtual void receiverStarted(Communication::Receiver & receiver);
      virtual void receiverStopped(Communication::Receiver & receiver);
      // Derived classes must implement serviceQueue
      // virtual bool serviceQueue(Communication::Receiver & receiver) = 0;

      ///////////////////////
      // Implement DataSource
      virtual bool getBuffer(const uchar *& buffer, size_t & size);

    protected:
      /// @brief Decode the contents of a memory buffer
      /// @param buffer points to the data.
      /// @param size is how many valid bytes of data are at *buffer.
      bool decodeBuffer(const unsigned char * buffer, size_t size);

    private:
      BasePacketAssembler & operator = (const BasePacketAssembler &);
      BasePacketAssembler(const BasePacketAssembler &);
      BasePacketAssembler();

    protected:
      /// Analyze the packet header.
      HeaderAnalyzer & packetHeaderAnalyzer_;
      /// Analyze the message header.
      HeaderAnalyzer & messageHeaderAnalyzer_;
      /// Receive the decoded information.
      Messages::ValueMessageBuilder & builder_;

      /// the current buffer being decoded.
      const unsigned char * currentBuffer_;
      /// the size of the current buffer.
      size_t currentSize_;

      /// How many buffers have been decoded.
      size_t messageCount_;
      /// How many bytes were in those buffers.
      size_t byteCount_;
      /// How many buffers should be decoded before stopping artificially.
      size_t messageLimit_;
    };

  }
}
#endif // BASEPACKETASSEMBLER_H
