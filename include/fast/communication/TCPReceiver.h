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
#ifndef TCPRECEIVER_H
#define TCPRECEIVER_H
// All inline, do not export.
//#include <Common/QuickFAST_Export.h>
#include "TCPReceiver_fwd.h"
#include <Communication/AsynchReceiver.h>
namespace QuickFAST
{
  namespace Communication
  {
    /// @brief Receive TCP Packets and pass them to a packet handler
    class TCPReceiver
      : public AsynchReceiver
    {
    public:
      /// @brief Construct given TCP connection information.
      /// @param hostName identifies the host to connect to
      /// @param port port service name or number
      TCPReceiver(
        const std::string & hostName,
        const std::string & port
        )
        : AsynchReceiver()
        , hostName_(hostName)
        , port_(port)
        , socket_(ioService_)
      {
      }

      /// @brief construct given shared io_service and TCP connection information
      /// @param ioService an ioService to be shared with other objects
      /// @param hostName is the name or dotted IP to connect to
      /// @param port port service name or number
      TCPReceiver(
        boost::asio::io_service & ioService,
        const std::string & hostName,
        const std::string & port
        )
        : AsynchReceiver(ioService)
        , hostName_(hostName)
        , port_(port)
        , socket_(ioService_)
      {
      }

      ~TCPReceiver()
      {
      }

      // Implement Receiver method
      virtual bool initializeReceiver()
      {
        bool ok = true;
        // generate a collection of possible endpoints for this host:port
        boost::asio::ip::tcp::resolver resolver(ioService_);
        boost::asio::ip::tcp::resolver::query query( hostName_, port_);
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

        // then iterate thru the collection until we find one that works.
        boost::system::error_code error;
        boost::asio::ip::tcp::resolver::iterator endIterator;
        bool connected = false;
        while(!connected && iterator != endIterator)
        {
          socket_.connect(*iterator, error);
          connected = !error;
          ++iterator;
        }
        if(!connected)
        {
          ok = false;
          std::stringstream msg;
          msg << "Cannot connect to [" << hostName_ << ':' << port_ << "]: " << error;
          (void) assembler_->reportCommunicationError(msg.str());
        }
        else
        {
          if(assembler_->wantLog(Common::Logger::QF_LOG_INFO))
          {
            std::stringstream msg;
            msg << "Connected to: " << hostName_ << ':' << port_;
            assembler_->logMessage(Common::Logger::QF_LOG_INFO, msg.str());
          }
        }
        return ok;
      }

      // Implement Receiver method
      virtual void stop()
      {
        Receiver::stop();
        try
        {
          socket_.close();
        }
        catch(...){}
      }

      /// Provide direct access to the internal asio socket.
      boost::asio::ip::tcp::socket & socket()
      {
        return socket_;
      }

      /// @brief send data to the socket (synchronous)
      ///
      /// Hint: DataDestination makes a good ConstBufferSequence
      /// @param buffers the source of the data.
      /// @returns the number of bytes transmitted.
      template<typename ConstBufferSequence>
      std::size_t send(const ConstBufferSequence & buffers)
      {
        return socket_.send(buffers);
      }

      /// @brief send data to the socket (synchronous)
      ///
      /// Hint: DataDestination makes a good ConstBufferSequence
      /// @param buffers the source of the data.
      /// @param flags see boost documentation for flags available
      /// @returns the number of bytes transmitted.
      template<typename ConstBufferSequence>
      std::size_t send(
        const ConstBufferSequence & buffers,
        boost::asio::ip::tcp::socket::message_flags flags)
      {
        return socket_.send(buffers, flags);
      }

      /// @brief send data to the socket (synchronous)
      ///
      /// Hint: DataDestination makes a good ConstBufferSequence
      /// @param buffers the source of the data.
      /// @param flags see boost documentation for flags available
      /// @param[out] ec set to the error if any.
      /// @returns the number of bytes transmitted.
      template<typename ConstBufferSequence>
      std::size_t send(
        const ConstBufferSequence & buffers,
        boost::asio::ip::tcp::socket::message_flags flags,
        boost::system::error_code & ec)
      {
        return socket_.send(buffers, flags, ec);
      }

      /// @brief send data to the socket (asynchronous)
      ///
      /// Hint: DataDestination makes a good ConstBufferSequence
      /// Important: ConstBufferSequence must not be changed until the send completes.
      /// @param buffers the source of the data.
      /// @param handler is called on I/O completion
      template<typename ConstBufferSequence, typename WriteHandler>
      void asyncSend(
        const ConstBufferSequence & buffers,
        WriteHandler handler)
      {
        socket_.async_send(buffers, handler);
      }

      /// @brief send data to the socket (asynchronous)
      ///
      /// Hint: DataDestination makes a good ConstBufferSequence
      /// Important: ConstBufferSequence must not be changed until the send completes.
      /// @param buffers the source of the data.
      /// @param flags see boost documentation for flags available
      /// @param handler is called on I/O completion
      template<typename ConstBufferSequence, typename WriteHandler>
      void asyncSend(
        const ConstBufferSequence & buffers,
        boost::asio::ip::tcp::socket::message_flags flags,
        WriteHandler handler)
      {
        socket_.async_send(buffers, flags, handler);
      }

    private:

      bool fillBuffer(LinkedBuffer * buffer, boost::mutex::scoped_lock& lock)
      {
        socket_.async_receive(
          boost::asio::buffer(buffer->get(), buffer->capacity()),
          boost::bind(&TCPReceiver::handleReceive,
            this,
            boost::asio::placeholders::error,
            buffer,
            boost::asio::placeholders::bytes_transferred)
          );
        return true;
      }

    private:
      std::string hostName_;
      std::string port_;
      boost::asio::ip::tcp::socket socket_;
    };
  }
}
#endif // TCPRECEIVER_H
