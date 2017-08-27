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
#ifndef FIELDINSTRUCTIONDECIMAL_H
#define FIELDINSTRUCTIONDECIMAL_H
#include <Codecs/FieldInstructionMantissa.h>
#include <Codecs/FieldInstructionExponent.h>
#include <Codecs/FieldInstruction.h>
#include <Common/Decimal.h>
namespace QuickFAST{
  namespace Codecs{
    ///@brief A FieldInstruction to encode/decode a Decimal data type.
    class QuickFAST_Export FieldInstructionDecimal : public FieldInstruction
    {
    public:
      /// @brief construct with a name and a namespace
      /// @param name is the local name
      /// @param fieldNamespace is the namespace to qualify this name
      FieldInstructionDecimal(
        const std::string & name,
        const std::string & fieldNamespace);

      /// @brief construct anonomous field instruction
      FieldInstructionDecimal();

      /// @brief a typical virtual destructor.
      virtual ~FieldInstructionDecimal();

      virtual void setPresence(bool mandatory);

      /// @brief Support &lt;mantissa> element.
      ///
      /// The mantissa field instruction may have special operations or properties.
      virtual void setMantissaInstruction(FieldInstructionPtr mantissa)
      {
        mantissaInstruction_ = mantissa;
        if(!bool(exponentInstruction_))
        {
          exponentInstruction_.reset(new FieldInstructionExponent(identity_->getLocalName() + "|decimal_exponent", identity_->getNamespace()));
          if(!isMandatory())
          {
            exponentInstruction_->setPresence(false);
          }
        }
      }

      /// @brief Get the mantissa field instruction.
      virtual bool getMantissaInstruction(FieldInstructionCPtr & mantissa) const
      {
        mantissa = mantissaInstruction_;
        return bool(mantissa);
      }

      /// @brief Support &lt;exponent> element.
      ///
      /// The exponent field instruction may have special operations or properties.
      virtual void setExponentInstruction(FieldInstructionPtr exponent)
      {
        exponentInstruction_ = exponent;
        if(!isMandatory())
        {
          exponentInstruction_->setPresence(false);
        }
        if(!bool(mantissaInstruction_))
        {
          mantissaInstruction_.reset(new FieldInstructionMantissa(identity_->name() + "|decimal_mantissa", identity_->getNamespace()));
        }
      }

      /// @brief Get the exponent field instruction.
      virtual bool getExponentInstruction(FieldInstructionCPtr & exponent) const
      {
        exponent = exponentInstruction_;
        return bool(exponent);
      }

      /// @brief Set the default/constant/copy, etc value.
      void setValue(exponent_t exponent, mantissa_t mantissa)
      {
        typedExponent_ = exponent;
        typedMantissa_ = mantissa;
        typedValueIsDefined_ = true;
        typedValue_ = Decimal(mantissa, exponent);
      }

      // virtual methods defined and documented in FieldInstruction
      virtual void decodeNop(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void decodeConstant(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void decodeDefault(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void decodeCopy(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void decodeDelta(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & builder) const;

      virtual void encodeNop(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void encodeConstant(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void encodeDefault(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void encodeCopy(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void encodeDelta(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & accessor) const;

      virtual void indexDictionaries(
        DictionaryIndexer & indexer,
        const std::string & dictionaryName,
        const std::string & typeName,
        const std::string & typeNamespace);

      virtual void finalize(TemplateRegistry & registry);
      virtual ValueType::Type fieldInstructionType()const;
      virtual void displayBody(std::ostream & output, size_t indent)const;

    private:
      void encodeDecimal(
        Codecs::DataDestination & destination,
        WorkingBuffer & buffer,
        exponent_t exponent,
        mantissa_t mantissa) const;

      void encodeNullableDecimal(
        Codecs::DataDestination & destination,
        WorkingBuffer & buffer,
        exponent_t exponent,
        mantissa_t mantissa) const;

    private:
      void interpretValue(const std::string & value);

    private:
      bool typedValueIsDefined_;
      exponent_t typedExponent_;
      mantissa_t typedMantissa_;
      // redundant, but efficient.  Must be Decimal(typedMantissa_, typedExponent_);
      Decimal typedValue_;
      FieldInstructionPtr mantissaInstruction_;
      FieldInstructionPtr exponentInstruction_;
    };
  }
}
#endif // FIELDINSTRUCTIONDECIMAL_H
