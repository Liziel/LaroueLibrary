#ifndef GameObject_hh__
# define GameObject_hh__

#include <vector>
#include <list>

#include "ctvty/component.hpp"

#include "ctvty/event.hpp"

#include "ctvty/object.hpp"

namespace ctvty {

  class GameObject : public Object {
  private:
    std::vector<ctvty::Component*>	components;

  private:
    bool				activation_state;
    std::vector<GameObject*>		childs;
    GameObject*				parent;

  protected:
    const std::string				tag;

  public:
    GameObject(const std::string name = "GameObject",
	       const std::string& _tag = "undefined",
	       GameObject* _parent = nullptr,
	       bool state = false)
      : Object(name), activation_state(state), parent(_parent), tag(_tag) {
      gameObjects.push_back(this);
    }

    ~GameObject() {
      parent->childs.remove([this] (GameObject* _comp) -> bool {return _comp == this;});
      gameObjects.remove([this] (GameObject* _comp) -> bool {return _comp == this;});
    }

  protected:
    virtual Object*				clone() const {
      GameObject*	clone = new GameObject(name + "(clone)", tag, parent);

      for (GameObject* child : childs)
	clone->childs.push_back(child->clone());
      for (Component* component : components)
	clone->components.push_back(component->clone());
      return (clone);
    }

    virtual void				intern_Destroy() {
      Object::intern_Destroy();

      for (GameObject* child : childs)
	child->intern_destroy();
      for (Component* component : components)
	component->intern_destroy();
      delete this;
    }

  public:
    void				AddComponent(ctvty::Component* component) {
      components.push_back(component);
    }

    void				BroadcastMessage(const std::string& methodName,
							 event::parameters::values params) {
      SendMessage(methodName, params);
      for (GameObject* child : childs)
	child->BroadcastMessage(methodName, params);
    }

    void				SendMessage(const std::string& methodName,
						    event::parameters::values params) {
      if (activation_state == false)
	return ;
      for (Component* component : components)
	if (component->doImplement(methodName))
	  component[methodName](params);
    }

    void				SendMessageUpwards(const std::string& methodName,
							   event::parameters::values params) {
      SendMessage(methodName, params);
      parent->sendMessageUpwards(methodName, params);
    }

  public:
    void				SetActive(bool state) {
      for (GameObject* child : childs)
	child->SetActive(state);
      activation_state = state;
    }

  public:
    template<typename component>
    component*				GetComponent() {
      for (ctvty::Component* _component : components) {
	if (dynamic_cast<component*>(_component) != nullptr)
	  return _component;
      }
      return (nullptr);
    }

    template<typename component>
    std::vector<component*>		GetComponents() {
      std::vector<component*>	_founds;

      for (ctvty::Component* _component : components) {
	if (dynamic_cast<component>(_component) != nullptr)
	  _founds.push_back(_component);
      }
      return (_founds);
    }

  public:
    template<typename component>
    component*				GetComponentInChildren() {
      for (ctvty::GameObject* gameObject : childs) {
	component* _component = gameObejct->GetComponentInChildren<component>();
	if (_component != nullptr)
	  return (_component);
      }
      return (nullptr);
    }

    template<typename component>
    std::vector<component*>		GetComponentsInChildren() {
      std::vector<component*> _founds;

      for (ctvty::GameObject* gameObject : childs) {
	std::vector<component*> _child_founds = gameObject->GetComponentsInChildren<component>();
	_founds.insert(_founds.end(), _child_founds.begin(), _child_founds.end());
      }
      return (_founds);
    }

  public:
    template<typename component>
    component*				GetComponentInParent() {
      component*   _found = parent->GetComponent<component>();
      if (!_found)
	return (parent->GetComponentInParent<compoenent>());
      return (_found);
    }
    template<typename component>
    component*				GetComponentsInParent() {
      std::vector<component*> _founds = parent->GetComponents<component>();
      std::vector<component*> _parents_found = parent->getComponentInParent<component>();

      _founds.insert(_founds.end(), _parents_found.begin(), _parents_found.end());
      return (_founds);
    }


  public:
    bool				CompareTag(const std::string& _tag) {
      return (_tag == tag);
    }

  private:
    static std::list<GameObject*>	gameObjects;

  public:
    static GameObject*			Find(const std::string& name) { 
      for (GameObject* gameObject : gameObjects)
	if (gameObject->name == name)
	  return (gameObject);
      return (nullptr);
    }

    static Gameobject*			FindGameObjectWithTag(const std::string& tag) {
      for (GameObject* gameObject : gameObjects)
	if (gameObject->CompareTag(tag))
	  return (gameObject);
      return (nullptr);
    }

    static std::vector<Gameobject>*	FindGameObjectsWithTag(const std::string& tag) {
      std::vector<GameObject>*		_founds;

      for (GameObject* gameObject : gameObjects)
	if (gameObject->CompareTag(tag))
	  _founds.push_back(gameObject);
      return (_founds);
    }

  };
};


#endif
