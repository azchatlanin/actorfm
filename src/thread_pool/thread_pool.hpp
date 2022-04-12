#pragma once

#include <vector>
#include <thread>

#include "logger/logger.hpp"

namespace actorfm
{
  class thread_pool
  {
    using Pool = std::vector<std::thread>;

    public:
      thread_pool();
      ~thread_pool();

      private:
        void worker();

      private:
        Pool pool;
        bool done { false };
  };
}
