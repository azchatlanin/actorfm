#pragma once

#include <memory>

#include "scheduler.hpp"

namespace actorfm
{
  class actor_controller
  {
    public: 
      actor_controller() : sch { std::make_unique<scheduler>() } {}

      static actor_controller& instance() { static actor_controller ac; return ac; }

      template<typename actor_t>
      void set_actro(actor_t& actor)
      {
        sch->set_actor(actor);
      }

    private: 
      std::unique_ptr<scheduler> sch;
  };
}
