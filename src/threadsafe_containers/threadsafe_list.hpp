#pragma once

#include <memory>
#include <mutex>
#include <condition_variable>

template<typename data_t>
class threadsafe_list
{
  public: 
    threadsafe_list() = default;
    ~threadsafe_list() 
    {

    };

    threadsafe_list(const threadsafe_list&) = delete;
    threadsafe_list& operator=(const threadsafe_list&) = delete;

  public: 
    void push_front(const data_t& value)
    {
      std::unique_ptr<node> new_node { new node { value } };
    }

    template <typename func>
    void for_each(func f)
    {
      node *current = &head;
      std::unique_lock lk { head.m };
      while (node* const next = current->next.get())
      {
        std::unique_lock next_lk { next->m };
        lk.unlock();
        f(*next->data);
        current = next;
        lk = std::move(next_lk);
      }
    }

    template <typename func>
    std::shared_ptr<data_t> find_first_if(func f)
    {
      node *current = &head;
      std::unique_lock lk { head.m };
      while (node *const next = current->next.get())
      {
        std::unique_lock next_lk { next->m };
        lk.unlock();
        if (f(*next->data))
          return next->data;
        current = next;
        lk = std::move(next_lk);
      }
      return std::shared_ptr<data_t>();
    }

    template <typename func>
    void remove_if(func f)
    {
      node *current = &head;
      std::unique_lock lk { head.m };
      while (node *const next = current->next.get())
      {
        std::unique_lock next_lk { next->m };
        if (f(*next->data))
        {
          std::unique_ptr<node> old_next = std::move(current->next);
          current->next = std::move(next->next);
          next_lk.unlock();
        }
        else
        {
          lk.unlock();
          current = next;
          lk = std::move(next_lk);
        }
      }
    }

  private: 
    struct node 
    {
      std::mutex m;
      std::shared_ptr<data_t> data;
      std::unique_ptr<node> next;
      node() : next() {}
      node(const data_t& value) : data { std::make_shared<data_t>(value) } {};
    };

    node head;
};
