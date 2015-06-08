#ifndef GameObject_hpp__
# define GameObject_hpp__

#include <map>
#include <string>
#include <vector>
#include <list>

#include "ctvty/event/properties.hpp"

#include "ctvty/object.hpp"

namespace ctvty {

  class Component;
  namespace component { class Transform; };
  class GameObject : public Object {
  private:
    /*
     * Components Vector storing the component attached to the GameObject
     * the two map allow a faster event dispatch:
     * events_map store the used and unused event listener in this object
     * child_events_map sotre the used unused event Listener in the childs objects
     */
    std::list<ctvty::Component*>		components;
    std::map<std::string, bool>			events_map;
    std::map<std::string, bool>			childs_events_map;

  private:
    /*
     * Activation State determine if the game Object should or should not receive any events
     */
    bool					activation_state;

  private:
    /*
     * All GameObject are a node of a n-sized tree
     */
    std::list<GameObject*>			childs;
    GameObject*					parent;
  public:
    inline bool					HasParent() { return parent != nullptr; }
    inline GameObject&				GetParent() { return *parent; }

  private:
    /*
     * All GameObject should have a Transform Component for their localization
     */
    component::Transform*					transform;
  public:
    component::Transform*					GetTransformation();

  protected:
    /*
     * A GameObject is definied by its tag, which can be shared with other gameObject (see FindWithTag)
     */
    const std::string				tag;

  private:
    /*
     * Static List of all GameObjects and all parent-less GameObject (on top of tree)
     */
    static std::list<GameObject*>	gameObjects;
    static std::list<GameObject*>	fathers;

  public:
    GameObject(const std::string&		name,
	       const std::string&		tag,
	       GameObject*			parent,
	       utils::Vector3D*			position,
	       utils::Quaternion*		rotation,
	       utils::Vector3D*			scale = new utils::Vector3D(1.f, 1.f, 1.f),
	       bool				state = false);

    GameObject(const std::string&		name = "GameObject",
	       const std::string&		tag = "undefined",
	       GameObject*			parent = nullptr,
	       component::Transform*		transform = nullptr,
	       bool				state = false);

    ~GameObject();


    GameObject(const serialization::Archive& __serial);

    void	Serialize(serialization::Archive& __serial) const override;

  public:
    /*
     * Inherited from ctvty::Object
     * intern method for Instantiate and Destroy
     */
    virtual Object*				clone() const;
    virtual void				intern_Destroy();

  public:
    /*
     * Everytime you want to set a father to a GameObject,
     * prefer call this setter, than to do it manualy
     */
    void					SetParent(GameObject*);
    void					SetParent(std::nullptr_t);
    void					AddChild(GameObject*);

  public:
    /*
     * Intern Method allowing faster treatment for events
     * Should ONLY be used by ctvty classes, user won't have to use it
     */
    void					SetEventListening(const std::string& eventName,
								  bool isListening,
								  GameObject* child = nullptr);

  public:
    /*
     * Instantiate and attach a new component of the given type
     *   ex:
     *    Collider2D component = this->AddComponent<Collider2D>() -> will create a Collider2D,
									attach it to this an return it
     */
    template<typename _component>
    Component*				AddComponent() {
      Component*	new_component = new _component(this);

      components.push_back(new_component);
      return (new_component);
    }


  public:
    /*
     * Message (Events) Parts
     * BroadCast send events + values to itself and its childs
     * SendMessage sned events + values to itself
     * SendMessageUpwards send events + values to itself and fathers
     */
    void					BroadcastMessage(const std::string& methodName,
								 event::parameters::values params);

    void					SendMessage(const std::string& methodName,
							    event::parameters::values params
								= event::parameters::PackValues());

    void					SendMessageUpwards(const std::string& methodName,
								   event::parameters::values params
									= event::parameters::PackValues());


  public:
    /*
     * Set the Activation State
     *  true -> this GameObject will receive events
     * false -> it will not 
     */
    void					SetActive(bool state);
    bool					IsActive();


  public:
    /*
     * Compare Its own Tag with the given tag (simple string == string -.-')
     */
    bool					CompareTag(const std::string& _tag);

  public:
    /*
     * Static Search Function, use the static gameObjects list
     * Find -> Find the first GameObject with the given name and return it
     * FindGameObjectWithTag -> Find the first GameObject with the given tag and return it
     * FindGameObjectsWIthTag -> make and vector of all GemeObject sharing the given tag and return it
     */
    static GameObject*				Find(const std::string& name);

    static GameObject*				FindGameObjectWithTag(const std::string& tag);

    static std::vector<GameObject*>		FindGameObjectsWithTag(const std::string& tag);



  public:
    /*
     * Return the orphan list, useful for dispatching event from top of each GameObject tree
     */
    static std::list<GameObject*>&		accessParentsGameObjects();



  public:
    /*

     * Component Search Methodes:
     *   Component -> return the first found
     *   ComponentS -> return an array of found components

     *   GetComponent(s) -> search in its own component
     *   GetComponent(s)InChildren() -> search in the children (deep search) components
     *	 GetComponent(s)InParent() -> search in the parent (deep search) components

     */
    template<typename component>
    component*					GetComponent() {
      component* c = nullptr;

      for (ctvty::Component* _component : components) {
	if ((c = dynamic_cast<component*>(_component)) != nullptr)
	  return c;
      }
      return (nullptr);
    }

    template<typename component>
    std::list<component*>			GetComponents() {
      std::list<component*>	_founds;

      for (ctvty::Component* _component : components) {
	if (dynamic_cast<component*>(_component) != nullptr)
	  _founds.push_back(dynamic_cast<component*>(_component));
      }
      return (_founds);
    }

  public:
    template<typename component>
    component*					GetComponentInChildren() {
      for (ctvty::GameObject* gameObject : childs) {
	component* _component = gameObject->GetComponentInChildren<component>();
	if (_component != nullptr)
	  return (_component);
      }
      return (nullptr);
    }

    template<typename component>
    std::list<component*>			GetComponentsInChildren() {
      std::list<component*> _founds;

      for (ctvty::GameObject* gameObject : childs) {
	std::list<component*> _child_founds = gameObject->GetComponentsInChildren<component>();
	_founds.insert(_founds.end(), _child_founds.begin(), _child_founds.end());
      }
      return (_founds);
    }

  public:
    template<typename component>
    component*					GetComponentInParent() {
      component*   _found = parent->GetComponent<component>();
      if (!_found)
	return (parent->GetComponentInParent<component>());
      return (_found);
    }

    template<typename component>
    component*					GetComponentsInParent() {
      std::list<component*> _founds = parent->GetComponents<component>();
      std::list<component*> _parents_found = parent->GetComponentsInParent<component>();

      _founds.insert(_founds.end(), _parents_found.begin(), _parents_found.end());
      return (_founds);
    }
  };
};


#endif
