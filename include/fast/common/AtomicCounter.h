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
#ifndef ATOMICCOUNTER_H
#define ATOMICCOUNTER_H
#include <Common/AtomicOps.h>
namespace QuickFAST
{
  ///@brief A counter that can be updated atomically.
  class AtomicCounter
  {
  public:
    ///@brief Construct with initial value.
    ///
    /// @param value is initial value of counter.
    AtomicCounter(long value = 0)
      : counter_(value)
    {
    }
    /// @brief Access current value as long
    /// Warning, not synchronized so you know this counter had the returned
    /// value at some point, but not necessarily when.
    inline
    operator const long()const
    {
      return counter_;
    }

    /// @brief Increment atomically
    inline
    long operator ++()
    {
      return atomic_increment_long(&counter_);
    }

    /// @brief Decrement atomically
    inline
    long operator --()
    {
      return atomic_decrement_long(&counter_);
    }

    /// @brief Atomically set the value assuming it hasn't changed from "expected"
    /// @param expected the value that the counter must start with if this is to work
    /// @param value the new value to be stored in the counter
    /// @returns true if the counter was updated.
    inline
    bool CAS(long expected, long value)
    {
      return CASLong(&counter_, expected, value);
    }
  private:
    volatile long counter_;
  };
}
#endif // ATOMICCOUNTER_H
