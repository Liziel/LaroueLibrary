#ifndef Component_hpp__
# define Component_hpp__

#include <map>
#include <string>

#include "ctvty/event/properties.hpp"
#include "ctvty/object.hpp"

namespace ctvty {

  class GameObject;
  namespace component {
    class Transform;
  };

  class Component : public Object {
  private:
    std::string							tag;

  protected:
    GameObject*							gameObject;
    component::Transform*					transform;

  protected:
    std::map< std::string, std::shared_ptr<event::receiver> >	registeredListener;

  public:
    Component(GameObject* parent,
	      const std::string& name);
    virtual ~Component();

  public:
    virtual void				AttachParent(GameObject*);
    virtual void				SetEnable(bool) {}

  protected:
    /*
     * allow a component to register the given listener with the given name

     * ex:
     *  this->RegisterListener("onAttack", myScript::onAttack);
     *  this->sendMessage("onAttack");
		// now the given methode onAttack will be called each time an "onAttack" event will be dispatched

     */
    template<typename child_class, typename ... listener_arguments>
    void			RegisterListener(const std::string& name,
						 void (child_class::*listener)(listener_arguments...)) {
      registeredListener
	.emplace(name,
		 std::shared_ptr<event::receiver>(event::receiver
				 ::create(this,
					  std::function<void ( child_class*, listener_arguments... )>(listener)
							 )));
      if (gameObject)
	gameObject->SetEventListening(name, true);
    }

  public:
    /*
     * will call the equivalent method in the parent GameObject
     * this->BroadcastMessage() == this->parent->BroadcastMessage();
     */
    void			BroadcastMessage(const std::string& methodName,
						 event::parameters::values params) const;

    template<typename ... parameters>
    void			BroadcastMessage(const std::string& methodName,
						 parameters ... p) const{
      std::vector<event::parameters::value*>	v(event::parameters::PackValues(p ... ));
      BroadcastMessage(methodName, v);
      for (event::parameters::value* _v : v)
	delete _v;
    }

    void			SendMessage(const std::string& methodName,
					    event::parameters::values params
						= event::parameters::PackValues());

    void			SendMessageUpwards(const std::string& methodName,
						   event::parameters::values params
							= event::parameters::PackValues());


  public:
    /*
     * will just check if the given tag is the same as the registered tag
     */
    bool			CompareTag(const std::string& tag);
    GameObject*			GetGameObject();

  public:
    /*
     * will call the equivalent method in the parent GameObject
     * this->GetComponent() == this->parent->GetComponent();
     */
    template<typename type>
    auto			GetComponent()
      -> decltype( GameObject().GetComponent<type>() ) {
      return this->gameObject->GetComponent<type>();
    }
    
    template<typename type>
    auto			GetComponentInChildren()
      -> decltype( GameObject().GetComponentInChildren<type>() ) {
      return this->gameObject->GetComponentInChildren<type>();
    }

    template<typename type>
    auto			GetComponentInParent()
      -> decltype( GameObject().GetComponent<type>() ) {
      return this->gameObject->GetComponent<type>();
    }

    template<typename type>
    auto			GetComponents()
      -> decltype( GameObject().GetComponents<type>() ) {
      return this->gameObject->GetComponents<type>();
    }
    
    template<typename type>
    auto			GetComponentsInChildren()
      -> decltype( GameObject().GetComponentsInChildren<type>() ) {
      return this->gameObject->GetComponentsInChildren<type>();
    }

    template<typename type>
    auto			GetComponentsInParent()
      -> decltype( GameObject().GetComponents<type>() ) {
      return this->gameObject->GetComponents<type>();
    }


  public:
    /*
     * Event listeners methodes,
     * Should only be used in intern
     */
    virtual bool		DoImplement(const std::string&);

    event::receiver&		operator [] (const std::string&);
  };
};


#endif
