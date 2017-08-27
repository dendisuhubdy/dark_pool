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
#ifndef QUICKFAST_EXPORT_H
#define QUICKFAST_EXPORT_H

// Compile time controls for library generation.  Define with /D or #define
// To produce or use a static library: #define QUICKFAST_HAS_DLL=0
//   Default is to produce/use a DLL
// While building the QUICKFAST_ library: #define QUICKFAST_BUILD_DLL
//   Default is to export symbols from a pre-built QUICKFAST DLL
//
// Within QUICKFAST use the QuickFAST_Export macro where a __declspec is needed.

#if defined (_WIN32)

#  if !defined (QUICKFAST_HAS_DLL)
#    define QUICKFAST_HAS_DLL 1
#  endif /* ! QUICKFAST_HAS_DLL */

#  if defined (QUICKFAST_HAS_DLL) && (QUICKFAST_HAS_DLL == 1)
#    if defined (QUICKFAST_BUILD_DLL)
#      define QuickFAST_Export __declspec(dllexport)
#    else /* QUICKFAST_BUILD_DLL */
#      define QuickFAST_Export __declspec(dllimport)
#    endif /* QUICKFAST_BUILD_DLL */
#  else /* QUICKFAST_HAS_DLL == 1 */
#    define QuickFAST_Export
#  endif /* QUICKFAST_HAS_DLL == 1 */

#  else /* !_WIN32 */

#    define QuickFAST_Export
#  endif /* _WIN32 */
#endif /* QUICKFAST_EXPORT_H */

