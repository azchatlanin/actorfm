#pragma once

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace actfm
{
  template<typename data_t>
  class threadsafe_queue
  {
    public: 
      threadsafe_queue() = default;
      threadsafe_queue(const threadsafe_queue& other)
      {
        std::lock_guard lg { other.mut };
        data = other.data;
      }

    public: 
      void push(data_t value)
      {
        std::lock_guard lg { mut };
        data.push(value);
        cv.notify_one();
      }

      void wait_and_pop(data_t& value)
      {
        std::unique_lock ul { mut };
        cv.wait(ul, [this] { return !data.empty(); });
        value = data.front();
        data.pop();
      }

      std::shared_ptr<data_t> wait_and_pop()
      {
        std::unique_lock ul { mut };
        cv.wait(ul, [this] { return !data.empty(); });
        std::shared_ptr<data_t> res { std::make_shared<data_t>(data.front()) };
        data.pop();
        return res;
      }

      bool try_pop(data_t& value)
      {
        std::lock_guard lg { mut };
        if (data.empty()) return false;
        value = data.front();
        data.pop();
        return true;
      }

      std::shared_ptr<data_t> try_pop()
      {
        std::lock_guard lg { mut };
        if (data.empty()) return std::shared_ptr<data_t>();
        std::shared_ptr<data_t> res { std::make_shared<data_t>(data.front()) };
        data.pop();
        return res;
      }

      bool empty() const
      {
        std::lock_guard lg { mut };
        return data.empty();
      }

    private: 
      mutable std::mutex mut;
      std::queue<data_t> data;
      std::condition_variable cv;
  };
}
