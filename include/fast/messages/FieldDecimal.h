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
#ifndef FIELDDECIMAL_H
#define FIELDDECIMAL_H
#include <Messages/Field.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief A field containing a scaled decimal number
    ///
    /// In the XML template file this field is described as &lt;decimal>
    class QuickFAST_Export FieldDecimal : public Field{
      /// @brief Construct a NULL field
      FieldDecimal();

      /// @brief Construct given a decimal value
      /// @param value is the initial value for the field.
      FieldDecimal(const Decimal & value);

      /// @brief Construct given a mantissa and exponent
      /// @param mantissa is the initial value for the mantissa
      /// @param exponent is the initial value for the exponent
      FieldDecimal(mantissa_t mantissa, exponent_t exponent);

    public:
      /// Identify the type of data associated with this field
      const static ValueType::Type fieldType = ValueType::DECIMAL;

    public:
      /// @brief Construct the field from a Decimal value
      /// @param value the value to be stored in the field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(const Decimal & value);

      /// @brief Construct the field from a Decimal value
      /// @param mantissa is the initial value for the mantissa
      /// @param exponent is the initial value for the exponent
      /// @returns a constant pointer to the immutable field
      static FieldCPtr create(mantissa_t mantissa, exponent_t exponent);

      /// @brief Construct a NULL field
      /// @returns a constant pointer to the immutable field
      static FieldCPtr createNull();

      /// @brief a typical virtual destructor.
      virtual ~FieldDecimal();

      // implement selected virtual methods from Field
      virtual const Decimal toDecimal() const;
      virtual void valueToStringBuffer()const;
    private:
    };
  }
}
#endif // FIELDDECIMAL_H
