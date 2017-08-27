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
#ifndef GENERICMESSAGEBUILDER_H
#define GENERICMESSAGEBUILDER_H
#include <Common/QuickFAST_Export.h>
#include <Codecs/MessageConsumer.h>
#include <Messages/MessageBuilder.h>
#include <Messages/Message_fwd.h>
#include <Messages/FieldSet_fwd.h>
#include <Messages/Sequence_fwd.h>
#include <Messages/Group_fwd.h>
namespace QuickFAST{
  namespace Codecs{
    class GenericSequenceBuilder;
    class GenericGroupBuilder;
    class GenericMessageBuilder;

    /// @brief Build a sequence during decoding
    class QuickFAST_Export GenericSequenceBuilder : public Messages::MessageBuilder
    {
    public:
      /// @brief Construct with reference to a parent
      explicit GenericSequenceBuilder(MessageBuilder * parent);

      /// @brief Virtual destructor
      virtual ~GenericSequenceBuilder();

      /// @brief prepare this sequence to receive fields.
      ///
      /// Not to be confused with startSequence which prepares a *nested* sequence
      /// within this one.
      /// @param identity identifies the sequence
      /// @param applicationType is the data type for a sequence entry
      /// @param applicationTypeNamespace qualifies applicationTYpe
      /// @param fieldCount is the maximum number of fields to expect in each entry
      /// @param lengthIdentity identifies the length field for this sequence
      /// @param length sets the value of the length field.
      void initialize(
        Messages::FieldIdentityCPtr & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t fieldCount,
        Messages::FieldIdentityCPtr & lengthIdentity,
        size_t length
        );

      /// @brief access the results of building the sequence
      const Messages::SequencePtr & getSequence()const;

      /// @brief start over on a new sequence
      void reset();

      //////////////////////////
      // Implement MessageBuilder

      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;
      virtual void addField(
        Messages::FieldIdentityCPtr & identity,
        const Messages::FieldCPtr & value);

      virtual Messages::MessageBuilder & startMessage(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual bool endMessage(Messages::ValueMessageBuilder & messageBuilder);
      virtual bool ignoreMessage(Messages::ValueMessageBuilder & messageBuilder);

      virtual Messages::MessageBuilder & startSequence(
        Messages::FieldIdentityCPtr & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t fieldCount,
        Messages::FieldIdentityCPtr & lengthIdentity,
        size_t length);

      virtual void endSequence(
        Messages::FieldIdentityCPtr & identity,
        Messages::ValueMessageBuilder & sequenceBuilder);

      virtual Messages::MessageBuilder & startSequenceEntry(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);

      virtual void endSequenceEntry(
        Messages::ValueMessageBuilder & entry);

      virtual Messages::MessageBuilder & startGroup(
        Messages::FieldIdentityCPtr & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) ;
      virtual void endGroup(
        Messages::FieldIdentityCPtr & identity,
        Messages::ValueMessageBuilder & groupBuilder);

      ///////////////////
      // Implement Logger

      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);

    private:
      const Messages::FieldSetPtr & fieldSet()const;

    private:
      Messages::MessageBuilder * parent_;
      Messages::FieldSetPtr fieldSet_;
      Messages::SequencePtr sequence_;
      boost::scoped_ptr<GenericSequenceBuilder> sequenceBuilder_;
      boost::scoped_ptr<GenericGroupBuilder> groupBuilder_;
    };

    /// @brief Build a Group during decoding
    class QuickFAST_Export GenericGroupBuilder : public Messages::MessageBuilder
    {
      GenericGroupBuilder();
      GenericGroupBuilder(const GenericGroupBuilder &);
      GenericGroupBuilder & operator = (const GenericGroupBuilder &);
    public:
      /// @brief Construct with reference to a parent
      explicit GenericGroupBuilder(MessageBuilder * parent);

      /// @brief Virtual destructor
      virtual ~GenericGroupBuilder();

      /// @brief Initialize this group.
      ///
      /// Not to be confused with startGroup which initializes
      /// group nested within this one.
      /// @param identity identifies the sequence
      /// @param applicationType is the data type for a sequence entry
      /// @param applicationTypeNamespace qualifies applicationTYpe
      /// @param size is the maximum number of fields to expect in each entry
      void initialize(
        Messages::FieldIdentityCPtr & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);

      /// @brief Return the group that was built
      /// @returns the newly built group.
      const Messages::GroupPtr & getGroup()const;

      /// @brief prepare to start over with a new group
      void reset();

      //////////////////////////
      // Implement MessageBuilder

      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;
      virtual void addField(
        Messages::FieldIdentityCPtr & identity,
        const Messages::FieldCPtr & value);
      virtual Messages::MessageBuilder & startMessage(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual bool endMessage(Messages::ValueMessageBuilder & messageBuilder);
      virtual bool ignoreMessage(Messages::ValueMessageBuilder & messageBuilder);

      virtual Messages::MessageBuilder & startSequence(
        Messages::FieldIdentityCPtr & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t fieldCount,
        Messages::FieldIdentityCPtr & lengthIdentity,
        size_t length);
      virtual void endSequence(
        Messages::FieldIdentityCPtr & identity,
        Messages::ValueMessageBuilder & sequenceBuilder);

      virtual Messages::MessageBuilder & startSequenceEntry(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual void endSequenceEntry(Messages::ValueMessageBuilder & entry);

      virtual Messages::MessageBuilder & startGroup(
        Messages::FieldIdentityCPtr & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) ;
      virtual void endGroup(
        Messages::FieldIdentityCPtr & identity,
        Messages::ValueMessageBuilder & groupBuilder);

      ///////////////////
      // Implement Logger

      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);

    private:
      const Messages::GroupPtr & groupPtr()const;
    private:
      Messages::MessageBuilder * parent_;
      Messages::FieldSetPtr fieldSetx_;
      Messages::GroupPtr group_;

      boost::scoped_ptr<GenericSequenceBuilder> sequenceBuilder_;
      boost::scoped_ptr<GenericGroupBuilder> groupBuilder_;
    };

    /// @brief Build a generic message during decoding
    class QuickFAST_Export GenericMessageBuilder : public Messages::MessageBuilder
    {
    public:
      /// @brief Construct given the consumer to receive the built messages.
      ///
      /// @param consumer will receive the messages after they are built.
      GenericMessageBuilder(MessageConsumer & consumer);

      /// @brief Virtual destructor
      virtual ~GenericMessageBuilder();

      //////////////////////////
      // Implement MessageBuilder
      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;
      virtual void addField(
        Messages::FieldIdentityCPtr & identity,
        const Messages::FieldCPtr & value);

      virtual Messages::MessageBuilder & startMessage(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual bool endMessage(Messages::ValueMessageBuilder & messageBuilder);
      virtual bool ignoreMessage(Messages::ValueMessageBuilder & messageBuilder);

      virtual Messages::MessageBuilder & startSequence(
        Messages::FieldIdentityCPtr & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t fieldCount,
        Messages::FieldIdentityCPtr & lengthIdentity,
        size_t length);
      virtual void endSequence(
        Messages::FieldIdentityCPtr & identity,
        Messages::ValueMessageBuilder & sequenceBuilder);

      virtual Messages::MessageBuilder & startSequenceEntry(
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size);
      virtual void endSequenceEntry(Messages::ValueMessageBuilder & entry);

      virtual Messages::MessageBuilder & startGroup(
        Messages::FieldIdentityCPtr & identity,
        const std::string & applicationType,
        const std::string & applicationTypeNamespace,
        size_t size) ;
      virtual void endGroup(
        Messages::FieldIdentityCPtr & identity,
        Messages::ValueMessageBuilder & groupBuilder);

      ///////////////////
      // Implement Logger

      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);


    private:
      const Messages::MessagePtr & message()const;
    private:
      MessageConsumer & consumer_;
      Messages::MessagePtr message_;
      GenericSequenceBuilder sequenceBuilder_;
      GenericGroupBuilder groupBuilder_;
    };
  }
}
#endif // GENERICMESSAGEBUILDER_H
