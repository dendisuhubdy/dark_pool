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

#ifndef PACKETSEQUENCINGASSEMBLER_H
#define PACKETSEQUENCINGASSEMBLER_H

#include "PacketSequencingAssembler_fwd.h"
#include <Codecs/BasePacketAssembler.h>
#include <Communication/BufferQueue.h>
#include <Communication/RecoveryFeed_fwd.h>

namespace QuickFAST
{
  namespace Codecs
  {
    /// @brief Service a Receiver's Queue when expecting packet boundaries to match message boundaries (UDP or Multicast)
    /// with (or without) block headers.
    class QuickFAST_Export PacketSequencingAssembler
      : public BasePacketAssembler
    {
    public:
      /// @brief Constuct the Assembler
      /// @param templateRegistry defines the decoding instructions for the decoder
      /// @param packetHeaderAnalyzer analyzes the header of each packet (if any)
      /// @param messageHeaderAnalyzer analyzes the header of each message (if any)
      /// @param builder receives the data from the decoder.
      /// @param lookAheadCount how many packets to look ahead before deciding there is a gap.
      /// @param recoveryFeed an object to recover the packets that should have been in a gap.
      PacketSequencingAssembler(
          TemplateRegistryPtr templateRegistry,
          HeaderAnalyzer & packetHeaderAnalyzer,
          HeaderAnalyzer & messageHeaderAnalyzer,
          Messages::ValueMessageBuilder & builder,
          size_t lookAheadCount,
          const Communication::RecoveryFeedPtr & recoveryFeed);

      virtual ~PacketSequencingAssembler();

      ///////////////////////////////////////
      // Implement Remaining Assembler method
      virtual bool serviceQueue(Communication::Receiver & receiver);

    private:
      /// @brief Initial processing of incoming packet from any source.
      void capturePacket(Communication::LinkedBuffer * buffer);
      /// @brief Ready to decode a packet
      void processPacket(Communication::LinkedBuffer * buffer);
      /// @brief Packet is no longer needed.  Return it to from whence it came.
      void releasePacket(Communication::LinkedBuffer * buffer);
      /// @brief Packet is beyond the look-ahead array.   Hang on to it for later.
      void addToDeferred(Communication::LinkedBuffer * buffer, sequence_t sequenceNumber);
      /// @brief Promote deferred packets to the look-ahead array if possible.
      /// @return true any were promoted.
      bool promoteDeferred();

      /// @brief find the sequence number of the first available packet after a gap.
      sequence_t findGapEnd()const;

      /// @brief Report a gap to the recovery feed.
      /// Either wait for recovery information to arrive, or skip over the gap.
      void handleGap();

    private:
      PacketSequencingAssembler & operator = (const PacketSequencingAssembler &);
      PacketSequencingAssembler(const PacketSequencingAssembler &);
      PacketSequencingAssembler();

    private:
      size_t lookAheadCount_;
      boost::scoped_array<Communication::LinkedBuffer *> lookAhead_;
      bool first_;
      sequence_t nextSequenceNumber_;
      bool gapWait_;
      sequence_t gapEnd_;

      Communication::BufferQueue deferredQueue_;
      Communication::BufferQueue recoveryIncoming_;
      Communication::RecoveryFeedPtr recoveryFeed_;

      Communication::Receiver * receiver_;

    };

  }
}
#endif // PACKETSEQUENCINGASSEMBLER_H
