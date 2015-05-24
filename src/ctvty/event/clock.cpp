#include <thread>
#include "ctvty/gameObject.hpp"
#include "ctvty/event/clock.hh"
#include "ctvty/rendering/renderer.hh"
#include "ctvty/event.hh"
#include "ctvty/application.hh"

namespace ctvty {
  namespace event {

    Clock::			Clock()
      : frame_length((int64_t)(1000 / 60)), lastFrameRatio(1), end(false) {}


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

    void				Clock::Start() {
      std::chrono::time_point<std::chrono::system_clock>	fixed_beg, fixed_loop;

      fixed_beg = std::chrono::high_resolution_clock::now();;
      while (!end) {
	fixed_loop = std::chrono::high_resolution_clock::now();
	if (fixed_loop - fixed_beg <= frame_length)
	  std::this_thread::sleep_for(frame_length - (fixed_loop - fixed_beg));
	else
	  lastFrameRatio = (fixed_loop - fixed_beg) / frame_length;
	fixedDeltaTime = std::chrono::high_resolution_clock::now() - fixed_beg;
	fixed_beg = std::chrono::high_resolution_clock::now();
	std::for_each(GameObject::accessParentsGameObjects().begin(),
		      GameObject::accessParentsGameObjects().end(),
		      [] (GameObject* gameObject) { gameObject->BroadcastMessage("FixedUpdate"); }
		      );
	Event::Refresh();
	for (DelayedAction* action : std::list<DelayedAction*>(delayedActions))
	  action->Refresh();

	{
	  std::list<GameObject*> fathers_copy(ctvty::GameObject::accessParentsGameObjects());
	  ctvty::rendering::Renderer::GetRenderer().Update();
	  std::for_each(fathers_copy.begin(),
			fathers_copy.end(),
			[this] (GameObject* gameObject) { if (!end) gameObject->BroadcastMessage("Update"); }
			);
	}

	{
	  std::list<GameObject*> fathers_copy(ctvty::GameObject::accessParentsGameObjects());
	  Event* e;
	  while ((e = Event::Eat()) != nullptr) {
	    if (e->type() == Event::Type::quit)
	      Application::Quit();
	    else
	      std::for_each(fathers_copy.begin(),
			    fathers_copy.end(),
			    [this] (GameObject* gameObject) { if (!end) gameObject->BroadcastMessage("OnGui"); }
			    );
	  }
	}

	{
	  ctvty::rendering::Renderer::GetRenderer().Update();
	  std::list<GameObject*> fathers_copy(ctvty::GameObject::accessParentsGameObjects());
	  for (std::size_t i = 0; i < ctvty::rendering::Renderer::GetRenderer().RegisteredCameras(); ++i) {
	    ctvty::rendering::Renderer::GetRenderer().Pre3DRendering(i);
	    std::for_each(fathers_copy.begin(),
			  fathers_copy.end(),
			  [this] (GameObject* gameObject) { if (!end) gameObject->BroadcastMessage("Render"); }
			  );
	    ctvty::rendering::Renderer::GetRenderer().PreHUDRendering(i);
 	  }
	  ctvty::rendering::Renderer::GetRenderer().MainHUDRendering();
	  ctvty::rendering::Renderer::GetRenderer().Flush();
	}
      }
    }
  };
};
