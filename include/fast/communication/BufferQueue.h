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
#ifndef BUFFERQUEUE_H
#define BUFFERQUEUE_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "BufferQueue_fwd.h"
#include <Communication/LinkedBuffer.h>

namespace QuickFAST
{
  namespace Communication
  {
    /// @brief No frills ordered collection of buffers: FIFO
    ///
    /// No internal synchronization.
    /// This object does not manage buffer lifetimes.  It assumes
    /// that buffers outlive the collection.
    class BufferQueue
    {
    public:
      BufferQueue()
        : head_()
        , tail_(&head_)
      {
      }

      ///@brief return true if empty
      bool isEmpty() const
      {
        return head_.link() == 0;
      }

      /// @brief Push a single buffer onto the queue
      /// @param buffer is the buffer to be pushed
      /// @returns true if this queue was empty before the push; and not empty afterward
      bool push(LinkedBuffer * buffer)
      {
        bool first = isEmpty();
        assert(buffer != 0);
        buffer->link(0);
        tail_->link(buffer);
        tail_ = buffer;
        return first;
      }


      /// @brief Push a single buffer onto the front of the queue
      /// @param buffer is the buffer to be pushed
      /// @returns true if this queue was empty before the push; and not empty afterward
      bool push_front(LinkedBuffer * buffer)
      {
        bool first = isEmpty();
        assert(buffer != 0);
        buffer->link(head_.link());
        head_.link(buffer);
        if(first)
        {
          tail_ = buffer;
        }
        return first;
      }

      /// @brief Push a list of buffers onto the queue.
      /// @param buffer is the first of a linked list of buffers to be added to the queue
      /// @returns true if this queue was empty before the push; and not empty afterward
      bool pushList(LinkedBuffer * buffer)
      {
        bool first = isEmpty();
        assert(buffer != 0);
        tail_->link(buffer);
        while(buffer->link() != 0)
        {
          buffer = buffer->link();
        }
        tail_ = buffer;
        return first;
      }

      /// @brief Push buffers from one queue to another.
      ///
      /// The source queue is empty after this call.
      /// @param queue the source queue.
      /// @returns true if this queue was empty before the push; and not empty afterward
      bool push(BufferQueue & queue)
      {
        bool first = isEmpty();
        LinkedBuffer * head = queue.head_.link();
        if(head != 0)
        {
          tail_->link(head);
          tail_ = queue.tail_;
        }
        queue.head_.link(0);
        queue.tail_ = & queue.head_;
        return first && !isEmpty();
      }

      /// @brief Pop the first entry from the queue
      /// @returns the buffer or 0 if the queue is empty.
      LinkedBuffer * pop()
      {
        LinkedBuffer * result = head_.link();
        if(result != 0)
        {
          head_.link(result->link());
          result->link(0);
          if(head_.link() == 0)
          {
            tail_ = &head_;
          }
        }
        return result;
      }

      /// @brief Pop the entire contents of the queue as a linked list.
      LinkedBuffer * popList()
      {
        LinkedBuffer * result = head_.link();
        head_.link(0);
        tail_ = &head_;
        return result;
      }

      /// @brief nondestructive access to the first item in the queue
      /// @returns zero if none
      LinkedBuffer * peek() const
      {
        return head_.link();
      }

      /// @brief nondestructive access to the last item in the queue.
      /// @returns zero if none
      LinkedBuffer *peek_tail() const
      {
        if(tail_ == & head_)
        {
          return 0;
        }
        return tail_;
      }

    private:
      LinkedBuffer head_;
      LinkedBuffer * tail_;
    };
  }
}
#endif // BUFFERQUEUE_H
