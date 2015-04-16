#include "ctvty/event/delayed.hh"
#include "ctvty/event/clock.hh"

namespace ctvty {
  namespace event {
    DelayedAction::		DelayedAction(float delay, std::function<void()> _callback, float repeat)
      : time((int64_t)(1000 * delay)), repeat((int64_t)(1000 * repeat)), callback(_callback),
	time_start(std::chrono::high_resolution_clock::now()), do_repeat(repeat != 0.f) {
      Clock::GetClock().AddDelayedAction(this);
    }

    
    DelayedAction::		~DelayedAction() {
      Clock::GetClock().RemoveDelayedAction(this);
    }

    void			DelayedAction::Refresh() {
      if (std::chrono::high_resolution_clock::now() - time_start < time)
	return ;
      if (!do_repeat) {
	callback();
	delete this;
	return ;
      }
      callback();
      time = repeat;
      time_start = std::chrono::high_resolution_clock::now();
    }
  };
};
