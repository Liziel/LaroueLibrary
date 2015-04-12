#include "ctvty/gameObject.hpp"
#include "ctvty/component.hpp"

namespace ctvty {
  /*
   * Ctor & Dtor
   */
  Component::		Component(GameObject* parent, const std::string& name)
    : Object(name), gameObject(parent), transform(nullptr /*parent->GetComponent<Transform>()*/) {
  }

  Component::		~Component() {

  }

  /*
   * Message Functions
   */
  void			Component::BroadcastMessage(const std::string& methodName,
					 event::parameters::values params) {
    gameObject->BroadcastMessage(methodName, params);
  }

  void			Component::SendMessage(const std::string& methodName,
				    event::parameters::values params) {
    gameObject->SendMessage(methodName, params);
  }


  void			Component::SendMessageUpwards(const std::string& methodName,
					   event::parameters::values params) {
    gameObject->SendMessageUpwards(methodName, params);
  }


  /*
   * Compare Tag
   */
  bool			Component::CompareTag(const std::string& _tag) {
    return _tag == this->tag;
  }

  /*
   * 
   */
};
