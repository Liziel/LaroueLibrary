#include "ctvty/event/broadcast.hh"
#include "ctvty/gameObject.hpp"
#include "ctvty/object.hpp"

namespace ctvty {
  namespace event {
    BroadCast::BroadCast(GameObject* _target, Way _way, const std::string& _message, parameters::values _values)
      : target(_target), way(_way), message(_message), values(_values) {}

    BroadCast::BroadCast(const std::string& _message, parameters::values _values)
      : BroadCast(nullptr, Way::down, _message, std::move(_values)) {}

    BroadCast::BroadCast(GameObject* _target)
      : BroadCast(_target, Way::deletion, "", {}) {}

    BroadCast::BroadCast(Object* _target)
      : BroadCast(nullptr, Way::deletion, "", {}) {deletion = (_target);}

    BroadCast::~BroadCast() {
      for (auto value : values)
	delete value;
    }


    void	BroadCast::Dispatch() {
      if (target == nullptr) {
	std::for_each(GameObject::accessParentsGameObjects().begin(),
		      GameObject::accessParentsGameObjects().end(),
		      [this] (GameObject* gameObject) {
			if (removed.size())
			  for (auto it = removed.begin(); removed.end() != it; ++it)
			    if (*it == gameObject) {
			      removed.erase(it);
			      return ;
			    }
			gameObject->BroadcastMessage(message, values);
		      });
	removed.clear();
      } else {
	switch (way) {
	case Way::self :
	  target->SendMessage(message, values);
	  break;
	case Way::up :
	  target->SendMessageUpwards(message, values);
	  break;
	case Way::down :
	  target->BroadcastMessage(message, values);
	  break;
	case Way::deletion :
	  if (target) {
	    target->intern_Destroy();
	    delete target;
	  } else {
	    deletion->intern_Destroy();
	    delete deletion;
	  }
	}
      }
    }

    bool	BroadCast::RemoveTarget(GameObject* removal) {
      if (target == nullptr)
	removed.push_back(removal);
      return removal == target;
    }

  };
};
