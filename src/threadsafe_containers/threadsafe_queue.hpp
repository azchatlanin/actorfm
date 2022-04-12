#pragma once

#include <memory>
#include <mutex>
#include <condition_variable>

namespace actorfm
{
  template<typename data_t>
  class threadsafe_queue
  {
    public: 
      threadsafe_queue() : head { new node }, tail { head.get() } {}

      threadsafe_queue(const threadsafe_queue&) = delete;
      threadsafe_queue& operator=(const threadsafe_queue&) = delete;

    public: 
      std::shared_ptr<data_t> try_pop()
      {
        std::unique_ptr<data_t> const old_head = try_pop_head();
        return old_head ? old_head->data:std::shared_ptr<data_t>();
      }

      bool try_pop(data_t& value)
      {
        std::unique_ptr<node> const old_head = try_pop_head(value);
        return old_head;
      }

      std::shared_ptr<data_t> wait_and_pop()
      {
        std::unique_ptr<node> const old_head = wait_pop_head();
        return old_head->data;
      }

      void wait_and_pop(data_t& value)
      {
        std::unique_ptr<node> const old_head = wait_pop_head(value);
      }

      void push(data_t new_value)
      {
        std::shared_ptr<data_t> new_data { std::make_shared<data_t>(std::move(new_value)) };
        std::unique_ptr<node> p { new node };
        {
          std::lock_guard tail_lock { tail_mutex };
          tail->data=new_data;
          node* const new_tail = p.get();
          tail->next = std::move(p);
          tail = new_tail;
        }
        cv.notify_one();
      }

      bool empty()
      {
        std::lock_guard head_lock { head_mutex };
        return (head == get_tail());
      }

    private: 
      struct node 
      {
        std::shared_ptr<data_t> data;
        std::unique_ptr<node> next;
      };

      std::condition_variable cv;
      std::mutex head_mutex;
      std::mutex tail_mutex;
      std::unique_ptr<node> head;
      node* tail;

    private:
      std::unique_ptr<node> try_pop_head()
      {
        std::lock_guard head_lock { head_mutex };
        if(head.get() == get_tail())
          return std::unique_ptr<node>();
        return pop_head();
      }

      std::unique_ptr<node> try_pop_head(data_t& value)
      {
        std::lock_guard head_lock { head_mutex };
        if(head.get() == get_tail())
          return std::unique_ptr<node>();
        value = std::move(*head->data);
        return pop_head();
      }

      std::unique_ptr<node> pop_head()
      {
        std::unique_ptr<node> const old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
      }

      std::unique_lock<std::mutex> wait_for_data()
      {
        std::unique_lock head_lock { head_mutex };
        cv.wait(head_lock, [&] { return head != get_tail(); });
        return head_lock;
      }

      std::unique_ptr<node> wait_pop_head()
      {
        std::unique_lock head_lock { wait_for_data() };
        return pop_head();
      }

      std::unique_ptr<node> wait_pop_head(data_t& value)
      {
        std::unique_lock head_lock { wait_for_data() };
        value = std::move(*head->data);
        return pop_head();
      }

      node* get_tail()
      {
        std::lock_guard tail_lock { tail_mutex };
        return tail;
      }
  };
}
