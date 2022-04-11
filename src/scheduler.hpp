#pragma once

#include <memory>

#include "thread_pool/thread_pool.hpp"

#include "logger/logger.hpp"

namespace actfm
{
  class scheduler 
  {
    public:
      scheduler() : th_pool { std::make_unique<thread_pool>() } 
      {


      }

      template<typename actor_t>
      void set_actor(actor_t& actor)
      {
        hack::log()("ref semantic");
      }

      template<typename actor_t>
      void set_actor(actor_t&& actor)
      {
        hack::log()("move semantic");
      }

    private: 
      std::unique_ptr<thread_pool> th_pool;
  };
}

