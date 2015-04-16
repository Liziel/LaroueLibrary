#ifndef Delayed_hh__
# define Delayed_hh__

#include <chrono>
#include <functional>

namespace ctvty {
  namespace event {
    class DelayedAction {
    private:
      std::chrono::milliseconds	time, repeat;
      std::function<void()>	callback;

      std::chrono::time_point<std::chrono::high_resolution_clock>	time_start;

      bool			do_repeat;

    public:
      DelayedAction(float delay, std::function<void()> callback, float repeat = 0);
      ~DelayedAction();

    public:
      void			Refresh();
    };
  };
};

#endif
