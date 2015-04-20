#include <iostream>

#include "ctvty/event/delayed.hh"
#include "ctvty/event/clock.hh"

namespace ctvty {
  namespace event {
    DelayedAction::		DelayedAction(std::function<void()> _callback, float delay, float repeat)
      : time((int64_t)(1000 * delay)), repeat((int64_t)(1000 * repeat)), callback(_callback),
	time_start(std::chrono::high_resolution_clock::now()), do_repeat(repeat != 0.f),
	has_finished(false) {
      Clock::GetClock().AddDelayedAction(this);
    }

    
    DelayedAction::		~DelayedAction() {
      Clock::GetClock().RemoveDelayedAction(this);
    }

    void			DelayedAction::Refresh() {
      if (std::chrono::high_resolution_clock::now() - time_start < time)
	return ;
      if (!do_repeat) {
	has_finished = true;
	callback();
	delete this;
	return ;
      }
      callback();
      time = repeat;
      time_start = std::chrono::high_resolution_clock::now();
    }

    bool			DelayedAction::HasFinished() { return has_finished; }

  };
};
