#include "ctvty/gameObject.hpp"
#include "ctvty/component.hpp"
namespace ctvty {

  REGISTER_FOR_SERIALIZATION(GameObject);

  /*
   * Ctor & Dtor
   */
  GameObject::			GameObject(const std::string name,
					   const std::string& _tag,
					   GameObject* _parent,
					   bool state)
    : Object(name), activation_state(state), parent(nullptr), tag(_tag) {
    SetParent(_parent);
    gameObjects.push_back(this);
  }

  GameObject::			GameObject(const serialization::Archive& __serial)
      : GameObject(__serial.exist("name") ? __serial["name"].as<std::string>() : "GameObject",
		   __serial.exist("tag") ? __serial["tag"].as<std::string>() : "undefined") {
      if (__serial.exist("childs"))
	__serial["childs"] & childs;
      for (GameObject* child : childs)
	child->SetParent(this);

      if (__serial.exist("components"))
	__serial["components"] & components;
      for (Component* component : components)
	component->AttachParent(this);
  }

  GameObject::			~GameObject() {
    SetParent(nullptr);
    gameObjects.remove_if([this] (GameObject* _comp) -> bool {return _comp == this;});
  }

  /*
   * Set the parent and refresh parents::events and refresh parents list
   */
  void				GameObject::SetParent(GameObject* _parent) {
    if (_parent == parent)  return ;
    if (parent != nullptr)
      fathers.remove_if( [ = ](GameObject* child) { return child == this; } );
    SetParent(nullptr);
    if (_parent == nullptr) 
      return ;
    parent->childs.push_back(this);
    std::for_each(events_map.begin(), events_map.end(), [&] (std::pair<std::string, bool> pair) {
	if (pair.second)
	  parent->SetEventListening(pair.first, true, this);
      });
    std::for_each(childs_events_map.begin(), childs_events_map.end(), [&] (std::pair<std::string, bool> pair) {
	if (pair.second)
	  parent->SetEventListening(pair.first, true, this);
      });
    parent = _parent;
  }

  void				GameObject::SetParent(std::nullptr_t) {
    if (parent == nullptr)
      return ;
    parent->childs.remove_if( [ = ](GameObject* child) { return child == this; } );
    std::for_each(events_map.begin(), events_map.end(), [&] (std::pair<std::string, bool> pair) {
	if (pair.second)
	  parent->SetEventListening(pair.first, false, this);
      });
    std::for_each(childs_events_map.begin(), childs_events_map.end(), [&] (std::pair<std::string, bool> pair) {
	if (pair.second)
	  parent->SetEventListening(pair.first, false, this);
      });
    parent = nullptr;
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

    SetParent(nullptr);
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
							      GameObject* child) {
    if (child == nullptr) {
      if (events_map[eventName] == isListening)
	return ;
      events_map[eventName] = isListening;      
    } else {
      if (isListening == childs_events_map[eventName])
	return ;

      if (isListening)
	childs_events_map[eventName] = true;
      else
	for (GameObject* _child : childs) {
	  bool child_implement = _child->childs_events_map[eventName];
	  
	  if (_child == child)
	    return ;
	  if (child_implement == true) {
	    childs_events_map[eventName] = true;
	    return ;
	  } else
	    childs_events_map[eventName] = false;
	}
    }

    if (parent != nullptr && isListening == true && events_map[eventName] == false)
      parent->SetEventListening(eventName, isListening, this);
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

  std::list<GameObject*>	GameObject::gameObjects({});
  std::list<GameObject*>	GameObject::fathers({});
};
