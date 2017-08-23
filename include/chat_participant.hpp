
// exchange headers here
#include "chat_room.hpp"
#include "chat_server.hpp"
#include "chat_session.hpp"
#include "chat_message.hpp"

typedef std::deque<chat_message> chat_message_queue;

class chat_participant
{
public:
  virtual ~chat_participant() {}
  virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;
