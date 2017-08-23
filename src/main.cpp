
//STL headers here
#include <iostream>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>

// Boost headers here
#include <boost/asio.hpp>

// exchange headers here
#include "chat_participant.hpp"
#include "chat_room.hpp"
#include "chat_server.hpp"
#include "chat_session.hpp"
#include "chat_message.hpp"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try
  {
    if (argc < 2)
    {
      std::cerr << "Usage: exchange <port> [<port> ...]\n";
      return 1;
    }

    boost::asio::io_service io_service;

    std::list<chat_server> servers;
    for (int i = 1; i < argc; ++i)
    {
      tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
      servers.emplace_back(io_service, endpoint);
    }

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
