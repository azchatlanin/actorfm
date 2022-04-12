#pragma once

#include <vector>
#include <thread>

namespace actorfm
{
  template<typename message_t, typename result_t>
  class actor 
  {
    public:
      virtual ~actor() {}

    public:
      virtual result_t set_message(message_t ms, actor* ac = nullptr) = 0;

      void send(message_t ms, actor* ac = nullptr)
      {}

      result_t expect(message_t ms, actor* ac = nullptr)
      {}
  };
}

