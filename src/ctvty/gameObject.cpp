#include "ctvty/gameObject.hpp"
#include "ctvty/component.hpp"
namespace ctvty {

  /*
   * Ctor & Dtor
   */
  GameObject::			GameObject(const std::string name,
					   const std::string& _tag,
					   GameObject* _parent,
					   bool state)
    : Object(name), activation_state(state), parent(_parent), tag(_tag) {
    gameObjects.push_back(this);
    if (parent == nullptr)
      fathers.push_back(this);
  }

  GameObject::			~GameObject() {
    parent->childs.remove_if([this] (GameObject* _comp) -> bool {return _comp == this;});
    gameObjects.remove_if([this] (GameObject* _comp) -> bool {return _comp == this;});
  }

  /*
   * Herited From ctvty::Object
   */
  Object*			GameObject::clone() const {
    GameObject*	clone = new GameObject(name + "(clone)", tag, parent);

    for (GameObject* child : childs)
      clone->childs.push_back((GameObject*)child->clone());
    for (Component* component : components)
      clone->components.push_back((Component*)component->clone());
    return (clone);
  }

  void				GameObject::intern_Destroy() {
    Object::intern_Destroy();

    for (GameObject* child : childs)
      child->intern_Destroy();
    for (Component* component : components)
      component->intern_Destroy();
    delete this;
  }


  /*
   * Component Management
   */
  void				GameObject::SetEventListening(const std::string& eventName,
						  bool isListening,
						  bool isChild) {
    if (isChild)
      childs_events_map[eventName] = isListening;
    else
      events_map[eventName] = isListening;

    if (isListening == false && events_map[eventName] == true)
      return ;
    else if (parent != nullptr && isListening != events_map[eventName])
      parent->SetEventListening(eventName, isListening, true);
  }

  void				GameObject::BroadcastMessage(const std::string& methodName,
						 event::parameters::values params) {
    SendMessage(methodName, params);
    if (childs_events_map[methodName] == false)
      return ;
    for (GameObject* child : childs)
      child->BroadcastMessage(methodName, params);
  }

  void				GameObject::SendMessage(const std::string& methodName,
							event::parameters::values params) {
    if (activation_state == false)
      return ;
    if (events_map[methodName] == false)
      return ;
    for (Component* component : components)
      if (component->DoImplement(methodName))
	(*component)[methodName](params);
  }

  void				GameObject::SendMessageUpwards(const std::string& methodName,
						   event::parameters::values params) {
    SendMessage(methodName, params);
    parent->SendMessageUpwards(methodName, params);
  }

  /*
   * GameObject Activation State
   */
  void				GameObject::SetActive(bool state) {
    for (GameObject* child : childs)
      child->SetActive(state);
    activation_state = state;
    if (state == true)
      SendMessage("Start");
  }

  /*
   * Intern Utilisation Methodes
   */
  bool				GameObject::CompareTag(const std::string& _tag) {
    return (_tag == tag);
  }


  /*
   * Static Member functions
   */
  GameObject*			GameObject::Find(const std::string& name) { 
    for (GameObject* gameObject : gameObjects)
      if (gameObject->name == name)
	return (gameObject);
    return (nullptr);
  }

  GameObject*			GameObject::FindGameObjectWithTag(const std::string& tag) {
    for (GameObject* gameObject : gameObjects)
      if (gameObject->CompareTag(tag))
	return (gameObject);
    return (nullptr);
  }

  std::vector<GameObject*>	GameObject::FindGameObjectsWithTag(const std::string& tag) {
    std::vector<GameObject*>		_founds;

    for (GameObject* gameObject : gameObjects)
      if (gameObject->CompareTag(tag))
	_founds.push_back(gameObject);
    return (_founds);
  }

  std::list<GameObject*>&	GameObject::accessParentsGameObjects() {
    return (fathers);
  }

};
