#include "ctvty/behaviour.hh"

namespace ctvty {
  Behaviour::	Behaviour(GameObject* parent,
		       const std::string& name)
    : Component(parent, name), enabled(true) {}

  void		Behaviour::SetEnable(bool state) {
    if (enabled == state)
      return ;
    if (state == false)
      if (DoImplement("OnDisable"))
	(*this)["OnDisable"]();
    if (state == true)
      if (DoImplement("OnEnable"))
	(*this)["OnEnable"]();
    enabled = state;
    for (std::pair< std::string, std::shared_ptr<event::receiver> > pair : registeredListener) {
      gameObject->SetEventListening(pair.first, state);
    }
  }

  bool		Behaviour::DoImplement(const std::string& name) {
    if (!enabled)
      return false;
    return registeredListener.find(name) != registeredListener.end();
  }

};
