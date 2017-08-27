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
#ifndef DECIMAL_H
#define DECIMAL_H
#include <Common/QuickFAST_Export.h>
#include <Common/Decimal_fwd.h>
#include <Common/Types.h>

namespace QuickFAST{
  /// @brief A scaled Decimal data type as specified in the FAST standard
  ///
  /// Values are represented as a base 10 exponent (-64 < exponent < 64)
  /// Values may be normalized or unnormalized.  The autonormalize flag
  /// determines whether normalization happens automatically after every operation.
  /// In a normalized value, the mantissa has no trailing zeros (base 10)
  class QuickFAST_Export Decimal
    : public boost::arithmetic<Decimal>
    , public boost::totally_ordered<Decimal>
  {
  public:
    /// @brief Construct a Decimal, defaulting to 0.0 autonormalized
    explicit Decimal(
      mantissa_t mantissa = 0,
      exponent_t exponent = 0,
      bool autoNormalize = true);
    /// @brief Copy construct a decimal
    Decimal(const Decimal & rhs);
    /// @brief Destruct a decimal
    ~Decimal();
    /// @brief Parse a decimal value from a string
    ///
    /// Supports only www.fff format. [no explicit exponent]
    void parse(const std::string & value);
    /// @brief Set the autonormalize flag.
    void setAutoNormalize(bool autoNormalize);
    /// @brief Set the mantissa directly
    void setMantissa(mantissa_t mantissa);
    /// @brief Set the exponent directly (multiplies or divides by a power of 10)
    void setExponent(exponent_t exponent);
    /// @brief Get the mantissa value directly
    mantissa_t getMantissa() const;
    /// @brief Get the exponent value directly
    exponent_t getExponent() const;

    /// @brief Convert the value to a double, with potential loss of precision
    operator double()const;

    /// @brief Convert the value to an www.ffff formatted string
    void toString(std::string & value)const;

    /// @brief Assignment
    Decimal & operator=(const Decimal & rhs);

    /// @brief less than comparison
    bool operator<(const Decimal & rhs) const;
    /// @brief equality comparison
    bool operator==(const Decimal & rhs) const;

    /// @brief in place addition
    Decimal& operator+=(const Decimal & rhs);
    /// @brief in place subtraction
    Decimal& operator-=(const Decimal & rhs);
    /// @brief in place multiplication
    Decimal& operator*=(const Decimal & rhs);
    /// @brief in place division
    Decimal& operator/=(const Decimal & rhs);

    /// @brief force normalization
    ///
    /// @param strict if true throw if normalization fails; else leave the number accurate but denormalized
    void normalize(bool strict = true);

    /// @brief denormalize to achieve a specific exponent
    ///
    /// Normalization will happen even if it means a loss of precision
    /// @param exponent is the desired exponent
    void denormalize(exponent_t exponent);

    /// @brief Nothrow; no allocate; constant time swap values.
    /// @param rhs is the Decimal with which to swap
    void swap(Decimal & rhs);

  private:
    void maximizeMantissa();

  private:
    mantissa_t mantissa_;
    exponent_t exponent_;
    bool autoNormalize_;
  };
}
#endif // DECIMAL_H
