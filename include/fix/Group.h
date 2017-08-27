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

#ifndef FIX_GROUP
#define FIX_GROUP

#ifdef _MSC_VER
#pragma warning( disable: 4786 )
#endif

#include "FieldMap.h"
#include "Fields.h"
#include <vector>

namespace FIX
{
/**
 * Base class for all %FIX repeating groups.
 *
 * A group consists of a count field, deliminator, and a sorting order.
 */
class Group : public FieldMap
{
public:
  Group( int field, int delim )
  : FieldMap( message_order( delim, 0 ) ),
    m_field( field ), m_delim( delim ) {}

  Group( int field, int delim, const int order[] )
  : FieldMap( order ), m_field( field ), m_delim( delim ) {}

  Group( int field, int delim, const message_order& order )
  : FieldMap( order ), m_field( field ), m_delim( delim ) {}

  Group( const Group& copy )
  : FieldMap( copy ), m_field( copy.m_field ), m_delim( copy.m_delim ) {}

  int field() const { return m_field; }
  int delim() const { return m_delim; }

  void addGroup( Group& group );
  void replaceGroup( unsigned num, Group& group );
  Group& getGroup( unsigned num, Group& group ) const throw( FieldNotFound );
  void removeGroup( unsigned num, Group& group );
  void removeGroup( Group& group );
  bool hasGroup( const Group& group );
  bool hasGroup( unsigned num, Group& group );

private:
  int m_field;
  int m_delim;
};
}

#endif //FIX_GROUP
