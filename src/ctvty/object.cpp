#include "ctvty/object.hpp"
#include "ctvty/event/clock.hh"
#include "ctvty/gameObject.hpp"
#include "ctvty/component/transform.hh"

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

    if ((rmv = dynamic_cast<GameObject*>(del)) != nullptr) {
      ctvty::event::Clock::GetClock().RemoveTarget(rmv);
      rmv->BroadcastMessage("OnDestroy", {});
    }
    ctvty::event::Clock::GetClock().AddBroadCast(del);
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
      ctvty::event::Clock::GetClock().RemoveTarget(gameObject);
    }
    return (product);
  }

  Object*			Object::Instantiate(Object* _template,
						    const ctvty::utils::Vector3D& position,
						    const ctvty::utils::Quaternion& rotation) {
    Object*	product;
    GameObject*	gameObject;

    product = _template->clone();
    if ((gameObject = dynamic_cast<GameObject*>(product)) != nullptr) {
      delete gameObject->GetTransformation();
      gameObject->SetTransformation(new component::Transform(gameObject, position, rotation));
      gameObject->SetActive(true);
      gameObject->BroadcastMessage("Awake", {});
      ctvty::event::Clock::GetClock().RemoveTarget(gameObject);
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
