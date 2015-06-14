#include "ctvty/gameObject.hpp"
#include "ctvty/component.hpp"
#include "ctvty/event/clock.hh"

namespace ctvty {
  /*
   * Ctor & Dtor
   */
  Component::		Component(GameObject* parent, const std::string& name)
    : Object(name), gameObject(parent) {
    if (parent)
      AttachParent(parent);
  }

  Component::		~Component() {

  }

  /*
   * Message Functions
   */
  void			Component::BroadcastAll(const std::string& methodName,
					 event::parameters::values params) const {
    ctvty::event::Clock::GetClock().AddBroadCast(methodName,
						 params);
  }

  void			Component::BroadcastMessage(const std::string& methodName,
					 event::parameters::values params) const {
    ctvty::event::Clock::GetClock().AddBroadCast(gameObject,
						 ctvty::event::BroadCast::Way::down,
						 methodName,
						 params);
  }

  void			Component::SendMessage(const std::string& methodName,
				    event::parameters::values params) {
    ctvty::event::Clock::GetClock().AddBroadCast(gameObject,
						 ctvty::event::BroadCast::Way::self,
						 methodName,
						 params);
  }


  void			Component::SendMessageUpwards(const std::string& methodName,
					   event::parameters::values params) {
    ctvty::event::Clock::GetClock().AddBroadCast(gameObject,
						 ctvty::event::BroadCast::Way::up,
						 methodName,
						 params);
  }


  /*
   * Compare Tag
   */
  bool			Component::CompareTag(const std::string& _tag) {
    return _tag == this->tag;
  }

  /*
   * Getter
   */
  GameObject*		Component::GetGameObject() {
    return gameObject;
  }

  /*
   *  Event Listener
   */
  void			Component::AttachParent(GameObject* _gameObject) {
    gameObject = _gameObject;
    transform = gameObject->GetTransformation();
    std::for_each(registeredListener.begin(), registeredListener.end(),
		  [ & ] (std::pair<std::string, std::shared_ptr<event::receiver> > pair) {
		    gameObject->SetEventListening(pair.first, true);
		  });
  }

  bool			Component::DoImplement(const std::string& methodeName) {
    return registeredListener.find(methodeName) != registeredListener.end();
  }

  event::receiver&	Component::operator [] (const std::string& methodeName) {
    return *(registeredListener[methodeName]);
  }

};
