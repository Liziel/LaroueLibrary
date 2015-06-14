#include <thread>
#include "ctvty/gameObject.hpp"
#include "ctvty/event/clock.hh"
#include "ctvty/rendering/renderer.hh"
#include "ctvty/event.hh"
#include "ctvty/application.hh"

namespace ctvty {
  namespace event {

    Clock::			Clock()
      :	FixedUpdate("FixedUpdate"),
	Update("Update"),
	OnGui("OnGui"),
	Render("Render"),
	frame_length((int64_t)(1000 / 60)), lastFrameRatio(1), end(false) {}

    Clock&				Clock::GetClock() {
      static Clock			clock;
      return clock;
    }

    void				Clock::SetMaxFramePerSeconds(short frames) {
      maxFrameSeconds = frames;
      frame_length = std::chrono::milliseconds(1000 / frames);
    }

    float				Clock::GetFixedDeltaTime() {
      return 1 / maxFrameSeconds; (void)fixedDeltaTime.count();
    }

    double				Clock::LastFrameRatio() {
      return lastFrameRatio;
    }

    void				Clock::AddDelayedAction(DelayedAction* action) {
      delayedActions.push_back(action);
    }

    void				Clock::RemoveDelayedAction(DelayedAction* action) {
      delayedActions.remove_if([action] (DelayedAction* comp) { return comp == action; });
    }

    void				Clock::End() {
      end = true;
    }

    void				Clock::RemoveTarget(GameObject* target) {
      FixedUpdate.RemoveTarget(target);
      Update.RemoveTarget(target);
      OnGui.RemoveTarget(target);
      Render.RemoveTarget(target);
      for (auto it = broadcasts.begin(); it != broadcasts.end(); ++it)
	if (it->RemoveTarget(target) && (!dispatching || it != broadcasts.begin()))
	  it = broadcasts.erase(it);
    }

    void				Clock::Start() {
      std::chrono::time_point<std::chrono::high_resolution_clock>	fixed_beg, fixed_loop;

      dispatching = false;
      fixed_beg = std::chrono::high_resolution_clock::now();
      while (!end) {
	fixed_loop = std::chrono::high_resolution_clock::now();
	if (fixed_loop - fixed_beg <= frame_length)
	  std::this_thread::sleep_for(frame_length - (fixed_loop - fixed_beg));
	else
	  lastFrameRatio = (fixed_loop - fixed_beg) / frame_length;
	fixedDeltaTime = std::chrono::high_resolution_clock::now() - fixed_beg;
	fixed_beg = std::chrono::high_resolution_clock::now();

	Event::Refresh();

	FixedUpdate.Dispatch();
	dispatching = true;
	while (broadcasts.size()) { broadcasts.front().Dispatch(); broadcasts.pop_front(); }
	dispatching = false;

	for (DelayedAction* action : std::list<DelayedAction*>(delayedActions))
	  action->Refresh();

	Update.Dispatch();
	dispatching = true;
	while (broadcasts.size()) { broadcasts.front().Dispatch(); broadcasts.pop_front(); }
	dispatching = false;

	{//Event Dispatching By ONGUI
	  Event* e;
	  while ((e = Event::Eat()) != nullptr) {
	    if (e->type() == Event::Type::quit)
	      Application::Quit();
	    else
	      OnGui.Dispatch();
	  }
	}

	dispatching = true;
	while (broadcasts.size()) { broadcasts.front().Dispatch(); broadcasts.pop_front(); }
	dispatching = false;

	{//Renderer Dispatching By RENDER
	  ctvty::rendering::Renderer::GetRenderer().Update();
	  for (std::size_t i = 0; i < ctvty::rendering::Renderer::GetRenderer().RegisteredCameras(); ++i) {
	    ctvty::rendering::Renderer::GetRenderer().Pre3DRendering(i);
	    Render.Dispatch();
	    ctvty::rendering::Renderer::GetRenderer().PreHUDRendering(i);
 	  }
	  ctvty::rendering::Renderer::GetRenderer().MainHUDRendering();
	  ctvty::rendering::Renderer::GetRenderer().Flush();
	}

	dispatching = true;
	while (broadcasts.size()) { broadcasts.front().Dispatch(); broadcasts.pop_front(); }
	dispatching = false;


      }
    }
  };
};
