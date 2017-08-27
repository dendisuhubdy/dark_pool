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

#ifndef HTML_BUILDER_H
#define HTML_BUILDER_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include <sstream>

namespace HTML
{
class TAG
{
public:
  TAG( const std::string& tag, std::ostream& stream )
  : m_tag( tag ), m_stream( stream ) 
  {
    m_stream << "<" << m_tag;
  }

  virtual ~TAG() 
  {
    m_stream << m_value.str();
    m_stream << "</" << m_tag << ">";
  }

  TAG& text()
  { m_stream << ">"; return *this; }
  TAG& text( const std::string& value )
  { m_value << value; text(); return *this; }
  TAG& text( int value )
  { m_value << value; text(); return *this; } 

 private:
  std::string m_tag;
  std::stringstream m_value;

 protected:
  std::ostream& m_stream;
};

class SPECIAL
{
 public:
  SPECIAL( const std::string& value, std::ostream& stream )
  {
    stream << "&" << value << ";";
  }
};

class A : public TAG
{
public:
  A( std::ostream& stream )
  : TAG( "A", stream ) {}

  A& href( const std::string& value )
  { m_stream << " href='" << value << "'"; return *this; }
};

class BODY : public TAG
{
public:
  BODY( std::ostream& stream )
  : TAG( "BODY", stream ) {}
};

class BR : public TAG
{
public:
  BR( std::ostream& stream )
  : TAG( "BR", stream ) {}
};

class CAPTION : public TAG
{
public:
  CAPTION( std::ostream& stream )
  : TAG( "CAPTION", stream ) {}
};

class CENTER : public TAG
{
public:
  CENTER( std::ostream& stream )
  : TAG( "CENTER", stream ) {}
};

class EM : public TAG
{
public:
  EM( std::ostream& stream )
  : TAG( "EM", stream ) {}
};

class H1 : public TAG
{
public:
  H1( std::ostream& stream )
  : TAG( "H1", stream ) {}
};

class H2 : public TAG
{
public:
  H2( std::ostream& stream )
  : TAG( "H2", stream ) {}
};

class HEAD : public TAG
{
public:
  HEAD( std::ostream& stream )
  : TAG( "HEAD", stream ) {}
};

class HR : public TAG
{
public:
  HR( std::ostream& stream )
  : TAG( "HR", stream ) {}
};

const char* NBSP = "&nbsp;";

class TABLE : public TAG
{
public:
  TABLE( std::ostream& stream )
  : TAG( "TABLE", stream ) {}

  TABLE& border( int value )
  { m_stream << " border='" << value << "'"; return *this; }
  TABLE& cellspacing( int value )
  { m_stream << " cellspacing='" << value << "'"; return *this; }
  TABLE& width( int value )
  { m_stream << " width='" << value << "%'"; return *this; }
};

class TD : public TAG
{
public:
  TD( std::ostream& stream )
  : TAG( "TD", stream ) {}

  TD& align( const std::string& value )
  { m_stream << " align='" << value << "'"; return *this; }
};

class TITLE : public TAG
{
public:
  TITLE( std::ostream& stream )
  : TAG( "TITLE", stream ) {}
};

class TR : public TAG
{
public:
  TR( std::ostream& stream )
  : TAG( "TR", stream ) {}
};
}

#endif
