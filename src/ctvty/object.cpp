#include "ctvty/object.hpp"
#include "ctvty/event/clock.hh"
#include "ctvty/gameObject.hpp"

namespace ctvty {

  /*
   * Ctor & Dtor
   */
  Object::			Object(const std::string& _name) : name(_name), destroyableOnLoad(true) {
    Objects.push_back(this);
  }

  Object::			~Object() {
    Objects.remove_if([this] (Object* _comp) -> bool {return _comp == this;});
  }

  std::list<Object*>		Object::Objects( {} );

  /*
   * Destroy
   */
  void				Object::Destroy(Object* del) {
    GameObject*			rmv;
    ctvty::event::Clock::GetClock().AddBroadCast(del);
    if ((rmv = dynamic_cast<GameObject*>(del)) != nullptr)
      ctvty::event::Clock::GetClock().RemoveTarget(rmv);
  }

  void				Object::Destroy(Object* del, float delay) {
    new event::DelayedAction([del] () -> void { del->intern_Destroy(); delete del; }, delay);
  }

  /*
   * Instantiate
   */
  Object*			Object::Instantiate(Object* _template) {
    Object*	product;
    GameObject*	gameObject;

    product = _template->clone();
    if ((gameObject = dynamic_cast<GameObject*>(product)) != nullptr) {
      gameObject->SetActive(true);
      gameObject->BroadcastMessage("Awake", {});
    }
    return (product);
  }

  /*
   * Destroyable when loading a new scene
   */
  void				Object::DontDestroyOnLoad() {
    destroyableOnLoad = false;
  }

  bool				Object::IsDestroyableOnLoad() const {
    return destroyableOnLoad;
  }
};
