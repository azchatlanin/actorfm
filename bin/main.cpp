#include "actorfm.hpp"

#include "logger/logger.hpp"

using result_t = std::vector<int>;

struct message 
{
  result_t data;
};

class sorting : public actfm::actor<message, result_t>
{
  public:
    result_t set_message(message ms, actfm::actor<message, result_t>* ac = nullptr) override
    {
      result_t res;

      if (ac)
      {}
      else 
      {
        std::sort(ms.data.begin(), ms.data.end());
      }
      return res;

    }
};

int main(int argc, char *argv[])
{
  hack::log()("run example");

  message ms;
  ms.data = { 1, 5, 7, 9, 2, 4, 6, 8 };

  sorting sort;
  sort.set_message(ms);
  
  actfm::actor_controller::instance().set_actro(sort);
  
  hack::log()("completed");
}
