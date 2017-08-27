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
#ifndef BYTESWAPPER_H
#define BYTESWAPPER_H
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>

namespace QuickFAST
{
  /// @brief a function object that returns true if this is a big endian machine
  union BigEndian
  {
  private:
    unsigned short asShort;
    unsigned char asBytes[sizeof(unsigned short)];
  public:
    BigEndian()
      : asShort(1)
    {
    }
    /// @brief Is this a big endian machine?
    /// @returns true if this is a big endian machine
    bool operator() ()const
    {
      return asBytes[0] == 0;
    }
  };

  /// @brief a helper function object to access data that may need to be byte swapped.
  class ByteSwapper
  {
  public:
    /// @brief construct and enable or disable swapping
    ///
    /// @param swap if true swapping occurs.
    explicit ByteSwapper(bool swap = false)
      : swap_(swap)
    {
    }

    /// @brief change the state of swap after-the-fact
    ///
    /// @param swap if true swapping occurs.
    void setSwap(bool swap)
    {
      swap_ = swap;
    }

    /// @brief conditionally swap an unsigned 16 bit integer
    ///
    /// @param v the value to be swapped
    /// @returns the swapped value
    uint16 operator()(uint16 v) const
    {
      if(swap_)
      {
        return (v >> 8) | (v << 8);
      }
      return v;
    }


    /// @brief conditionally swap a signed 16 bit integer
    ///
    /// @param v the value to be swapped
    /// @returns the swapped value
    int16 operator()(int16 v) const
    {
      if(swap_)
      {
        return ((v >> 8) & 0xFF) | (v << 8);
      }
      return v;
    }


    /// @brief conditionally swap an unsigned 32 bit integer
    ///
    /// @param v the value to be swapped
    /// @returns the swapped value
    uint32 operator()(uint32 v) const
    {
      if(swap_)
      {
        return ((v >> 24) & 0x000000FF)
             | ((v >> 8)  & 0x0000FF00)
             | ((v << 8)  & 0x00FF0000)
             | ((v << 24) & 0xFF000000)
             ;
      }
      return v;
    }


    /// @brief conditionally swap a signed 32 bit integer
    ///
    /// @param v the value to be swapped
    /// @returns the swapped value
    int32 operator()(int32 v) const
    {
      if(swap_)
      {
        return ((v >> 24) & 0x000000FF)
             | ((v >> 8)  & 0x0000FF00)
             | ((v << 8)  & 0x00FF0000)
             | ((v << 24) & 0xFF000000)
             ;
      }
      return v;
    }

    /// @brief Test the endianness of this machine.
    /// @returns true if big-endian.
    static bool isBigEndian()
    {
      static BigEndian nativeBigEndian;
      return nativeBigEndian();
    }

  private:

    bool swap_;
  };

}
#endif // BYTESWAPPER_H
