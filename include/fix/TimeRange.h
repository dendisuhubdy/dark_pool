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

#ifndef FIX_TIMERANGE_H
#define FIX_TIMERANGE_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "FieldTypes.h"

namespace FIX
{
/// Keeps track of when session is active
class TimeRange
{
public:
  TimeRange( const UtcTimeOnly& startTime, const UtcTimeOnly& endTime,
               int startDay = -1, int endDay = -1 );

  TimeRange( const LocalTimeOnly& startTime, const LocalTimeOnly& endTime,
               int startDay = -1, int endDay = -1 );

  static bool isInRange( const UtcTimeOnly& start,
                         const UtcTimeOnly& end,
                         const DateTime& time )
  {
    return isInRange
      ( (DateTime)start, (DateTime)end, (DateTime)time );
  }

  static bool isInRange( const UtcTimeOnly& startTime,
                         const UtcTimeOnly& endTime,
                         int startDay,
                         int endDay,
                         const DateTime& time )
  {
    return isInRange
      ( (DateTime)startTime, (DateTime)endTime, 
        startDay, endDay, 
        (DateTime)time );
  }

  static bool isInSameRange( const UtcTimeOnly& start,
                             const UtcTimeOnly& end,
                             const DateTime& time1,
                             const DateTime& time2 )
  {
    return isInSameRange
      ( (DateTime)start, (DateTime)end, 
        (DateTime)time1, (DateTime)time2 );
  }

  static bool isInSameRange( const UtcTimeOnly& startTime,
                             const UtcTimeOnly& endTime,
                             int startDay,
                             int endDay,
                             const DateTime& time1,
                             const DateTime& time2 )
  {
    return isInSameRange
      ( (DateTime)startTime, (DateTime)endTime, 
        startDay, endDay, 
        (DateTime)time1, (DateTime)time2 );
  }

  static bool isInRange( const LocalTimeOnly& start,
                         const LocalTimeOnly& end,
                         const DateTime& time )
  {
    return isInRange
      ( (DateTime)start, (DateTime)end, (DateTime)time );
  }

  static bool isInRange( const LocalTimeOnly& startTime,
                         const LocalTimeOnly& endTime,
                         int startDay,
                         int endDay,
                         const DateTime& time )
  {
    return isInRange
      ( (DateTime)startTime, (DateTime)endTime, 
        startDay, endDay, 
        (DateTime)time );
  }

  static bool isInSameRange( const LocalTimeOnly& start,
                             const LocalTimeOnly& end,
                             const DateTime& time1,
                             const DateTime& time2 )
  {
    return isInSameRange
      ( (DateTime)start, (DateTime)end, 
        (DateTime)time1, (DateTime)time2 );
  }

  static bool isInSameRange( const LocalTimeOnly& startTime,
                             const LocalTimeOnly& endTime,
                             int startDay,
                             int endDay,
                             const DateTime& time1,
                             const DateTime& time2 )
  {
    return isInSameRange
      ( (DateTime)startTime, (DateTime)endTime, 
        startDay, endDay, 
        (DateTime)time1, (DateTime)time2 );
  }

private:
  static bool isInRange( const DateTime& start,
                         const DateTime& end,
                         const DateTime& time );

  static bool isInRange( const DateTime& startTime,
                         const DateTime& endTime,
                         int startDay,
                         int endDay,
                         const DateTime& time );

  static bool isInSameRange( const DateTime& start,
                             const DateTime& end,
                             const DateTime& time1,
                             const DateTime& time2 );

  static bool isInSameRange( const DateTime& startTime,
                             const DateTime& endTime,
                             int startDay,
                             int endDay,
                             const DateTime& time1,
                             const DateTime& time2 );
public:
  bool isInRange( const DateTime& dateTime )
  {
    if( m_startDay < 0 && m_endDay < 0 )
      return isInRange( m_startTime, m_endTime, dateTime );
    else
      return isInRange
        ( m_startTime, m_endTime, m_startDay, m_endDay, dateTime );
  }

  bool isInSameRange( const UtcTimeStamp& time1, const UtcTimeStamp& time2 )
  {
    return isInSameRange( (DateTime)time1, (DateTime)time2 );
  }

  bool isInSameRange( const LocalTimeStamp& time1, const LocalTimeStamp& time2 )
  {
    return isInSameRange( (DateTime)time1, (DateTime)time2 );
  }

private:
  bool isInSameRange( const DateTime& time1, const DateTime& time2 )
  {
    if( m_startDay < 0 && m_endDay < 0 )
      return isInSameRange( m_startTime, m_endTime, time1, time2 );
    else
      return isInSameRange
        ( m_startTime, m_endTime, m_startDay, m_endDay, time1, time2 );
  }

  UtcTimeOnly m_startTime;
  UtcTimeOnly m_endTime;
  int m_startDay;
  int m_endDay;
};
}

#endif
