// Copyright (c) 2001-2010 quickfixengine.org  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the
//    distribution.
//
// 3. The end-user documentation included with the redistribution,
//    if any, must include the following acknowledgment:
//      "This product includes software developed by
//       quickfixengine.org (http://www.quickfixengine.org/)."
//   Alternately, this acknowledgment may appear in the software itself,
//   if and wherever such third-party acknowledgments normally appear.
//
// 4. The names "QuickFIX" and "quickfixengine.org" must
//    not be used to endorse or promote products derived from this
//    software without prior written permission. For written
//    permission, please contact ask@quickfixengine.org
//
// 5. Products derived from this software may not be called "QuickFIX",
//    nor may "QuickFIX" appear in their name, without prior written
//    permission of quickfixengine.org
//
// THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.  IN NO EVENT SHALL QUICKFIXENGINE.ORG OR
// ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

#pragma once

#include "Utility.h"
#include "CallStack.h"
#include <math.h>

#ifndef _MSC_VER
#include <pthread.h>
#include <cmath>
#endif

namespace FIX
{
/// Portable implementation of an event/conditional mutex
class Event
{
public:
  Event()
  {
    pthread_mutex_init( &m_mutex, 0 );
    pthread_cond_init( &m_event, 0 );
  }

  ~Event()
  {
    pthread_cond_destroy( &m_event );
    pthread_mutex_destroy( &m_mutex );
  }

  void signal()
  {
    pthread_mutex_lock( &m_mutex );
    pthread_cond_broadcast( &m_event );
    pthread_mutex_unlock( &m_mutex );
  }

  void wait( double s )
  {
    pthread_mutex_lock( &m_mutex );
    timespec time, remainder;
    double intpart;
    time.tv_nsec = (long)(modf(s, &intpart) * 1e9);
    time.tv_sec = (int)intpart;
    pthread_cond_timedwait( &m_event, &m_mutex, &time );
    pthread_mutex_unlock( &m_mutex );
  }

private:
  pthread_cond_t m_event;
  pthread_mutex_t m_mutex;
};
}
