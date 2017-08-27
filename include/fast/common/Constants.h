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
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <Common/Types.h>

namespace QuickFAST{
  static const uchar stopBit = uchar('\x80');
  static const uchar dataBits = uchar('\x7F');
  static const uchar signBit = uchar('\x40');
  static const unsigned int dataShift(7u);
  static const unsigned int byteSize(8u);

  static const uchar zeroIntegerNonnullable = uchar('\x80');
  static const uchar zeroIntegerNullable = uchar('\x81');

  static const uchar nullInteger = uchar('\x80');
  static const uchar nullAscii = uchar('\x80');
  static const uchar nullBlob = uchar('\x80');
  static const uchar nullDecimal = uchar('\x80');

  static const uchar nullableStringPreamble = uchar('\0');
  static const uchar emptyString = uchar('\x80');
  static const uchar leadingZeroBytePreamble = uchar('\0');

  static const uchar ASCII_SOH = '\x01';
  static const uchar ASCII_ETX = '\x03';
  static const uchar ASCII_US = '\x1F';

}
#endif // CONSTANTS_H
