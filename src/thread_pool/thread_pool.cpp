#include "thread_pool.hpp"

namespace actfm
{
  thread_pool::thread_pool()
  {
    const auto thread_count = std::thread::hardware_concurrency();

    try 
    {
      for (auto i = 0u; i < thread_count; ++i)
        pool.emplace_back(std::thread(&thread_pool::worker, this));
    } 
    catch (std::exception& e) 
    {
      hack::log()(e.what());
    }
  }

  thread_pool::~thread_pool()
  {
    done = true;
    hack::log()("thread_pool completed");
  }

  void thread_pool::worker()
  {
    while (!done)
    {
      hack::log()("run thread");
    }
  };
}
