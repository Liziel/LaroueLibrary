#ifndef Component_hpp__
# define Component_hpp__

#include <map>
#include <string>

#include "ctvty/event.hpp"
#include "ctvty/object.hpp"

namespace ctvty {

  class GameObject;
  class Transform;

  class Component : public Object {
  private:
    std::string			tag;

  private:
    Transform*			transform;
    GameObject*			gameObject;

  public:
    void			SetTransform(Transform*);
    void			SetGameObject(GameObject*);

  public:
    Component();
    ~Component();

  public:
    void			BroadcastMessage(const std::string& methodName,
						 event::parameters::values params);
    
    void			CompareTag(const std::string& tag);

    void			GetComponent();

    void			GetComponentInChildren();

    void			GetComponentInParent();

    void			GetComponents();

    void			GetComponentsInParent();

    void			SendMessage();

    void			SendMessageUpwards();

  public:
    bool			doImplement(const std::string&);

  public:
    event::receiver&		operator [] (const std::string&);
  };
};


#endif
