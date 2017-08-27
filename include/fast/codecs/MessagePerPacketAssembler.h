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
#ifndef MESSAGEPERPACKETASSEMBLER_H
#define MESSAGEPERPACKETASSEMBLER_H

#include "MessagePerPacketAssembler_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Codecs/BasePacketAssembler.h>

namespace QuickFAST
{
  namespace Codecs
  {
    /// @brief Service a Receiver's Queue when expecting packet boundaries to match message boundaries (UDP or Multicast)
    /// with (or without) block headers.
    class QuickFAST_Export MessagePerPacketAssembler
      : public BasePacketAssembler
    {
    public:
      /// @brief Constuct the Assembler
      /// @param templateRegistry defines the decoding instructions for the decoder
      /// @param packetHeaderAnalyzer analyzes the header of each packet (if any)
      /// @param messageHeaderAnalyzer analyzes the header of each message (if any)
      /// @param builder receives the data from the decoder.
      MessagePerPacketAssembler(
          TemplateRegistryPtr templateRegistry,
          HeaderAnalyzer & packetHeaderAnalyzer,
          HeaderAnalyzer & messageHeaderAnalyzer,
          Messages::ValueMessageBuilder & builder);

      virtual ~MessagePerPacketAssembler();

      ///////////////////////////////////////
      // Implement Remaining Assembler method
      virtual bool serviceQueue(Communication::Receiver & receiver);

    private:
      MessagePerPacketAssembler & operator = (const MessagePerPacketAssembler &);
      MessagePerPacketAssembler(const MessagePerPacketAssembler &);
      MessagePerPacketAssembler();
    };
  }
}
#endif // MESSAGEPERPACKETASSEMBLER_H
