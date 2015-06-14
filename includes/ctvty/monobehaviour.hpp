#ifndef MonoBehaviour_hpp__
# define MonoBehaviour_hpp__

/* AND HERE IS THE MONSTER !! */

# include "ctvty/behaviour.hh"
# include "ctvty/event/delayed.hh"

# define ATTACHED_TO_EVENT(__event__, __parameters__)			\
  template<typename type>						\
  auto attach_to_event_ ## __event__ (type* _this)			\
  -> decltype( _this -> __event__ ( __parameters__ ), bool() )		\
  { this->Component::RegisterListener( # __event__ ,			\
				       &type :: __event__);		\
    (void)_this; return true;}						\
									\
  bool attach_to_event_ ## __event__ ( ... )				\
  { return false; }							\
  

# define ATTACH_THIS_TO(__event__)					\
  attach_to_event_ ## __event__ (dynamic_cast<child_class*>(this))	\

namespace ctvty {
  namespace component { class Collider; };
  namespace utils { struct Collision; };

  template <typename child_class>
  class MonoBehaviour : public Behaviour {
  private:
    /*
     *Invoke -> do delayed call
     */
    std::list< event::DelayedActionContainer< std::list >* >	invoked;

  public:
    template<typename ... arguments>
    void			Invoke(void (child_class::*invokable)(arguments...), float time, arguments ... args) {
      invoked.push_back(new event::DelayedActionContainer< std::list >(invoked,
						   std::bind(invokable,
							     dynamic_cast<child_class*>(this),
							     args ... ),
						   time)
			);
    }

    template<typename ... arguments>
    void			InvokeRepeating(void (child_class::*invokable)(arguments...),
						float time, float repeat,
						arguments ... args) {
      invoked.push_back(new event::DelayedActionContainer< std::list >(invoked,
						   std::bind(invokable,
							     dynamic_cast<child_class*>(this),
							     args ... ),
						   time, repeat)
			);
    }

    void			CancelInvoke() {
      for (auto& invoking : std::list< event::DelayedActionContainer< std::list >* >(invoked)) {
	std::cout << "deleting " << invoking << std::endl;
	delete invoking;
	invoking = nullptr;
      }
      invoked.clear();
    }

  public:
    MonoBehaviour(GameObject* parent, const std::string& name)
      : Behaviour(parent, name), invoked({}) {
      ATTACH_THIS_TO(Awake);
      ATTACH_THIS_TO(Start);
      ATTACH_THIS_TO(Update);
      ATTACH_THIS_TO(Render);
      ATTACH_THIS_TO(FixedUpdate);
      ATTACH_THIS_TO(OnApplicationQuit);
      ATTACH_THIS_TO(OnBecomeInvisible);
      ATTACH_THIS_TO(OnBecomeVisible);
      ATTACH_THIS_TO(OnCollisionEnter);
      ATTACH_THIS_TO(OnCollisionExit);
      ATTACH_THIS_TO(OnCollisionStay);
      ATTACH_THIS_TO(OnTriggerEnter);
      ATTACH_THIS_TO(OnTriggerExit);
      ATTACH_THIS_TO(OnTriggerStay);
      ATTACH_THIS_TO(OnDestroy);
      ATTACH_THIS_TO(OnDisable);
      ATTACH_THIS_TO(OnEnable);
      ATTACH_THIS_TO(OnGui);
      ATTACH_THIS_TO(OnLoadScene);
    }

  private:
    ATTACHED_TO_EVENT(Awake,);
    ATTACHED_TO_EVENT(Start,);
    ATTACHED_TO_EVENT(Update,);
    ATTACHED_TO_EVENT(Render,);
    ATTACHED_TO_EVENT(FixedUpdate,);
    ATTACHED_TO_EVENT(OnApplicationQuit,);
    ATTACHED_TO_EVENT(OnBecomeInvisible,);
    ATTACHED_TO_EVENT(OnBecomeVisible,);
    ATTACHED_TO_EVENT(OnCollisionEnter,		std::declval<const utils::Collision*>());
    ATTACHED_TO_EVENT(OnCollisionExit,		std::declval<const utils::Collision*>());
    ATTACHED_TO_EVENT(OnCollisionStay,		std::declval<const utils::Collision*>());
    ATTACHED_TO_EVENT(OnTriggerEnter,		std::declval<const component::Collider*>());
    ATTACHED_TO_EVENT(OnTriggerExit,		std::declval<const component::Collider*>());
    ATTACHED_TO_EVENT(OnTriggerStay,		std::declval<const component::Collider*>());
    ATTACHED_TO_EVENT(OnLoadScene,		std::declval<const std::string&>());
    ATTACHED_TO_EVENT(OnDestroy,);
    ATTACHED_TO_EVENT(OnDisable,);
    ATTACHED_TO_EVENT(OnEnable,);
    ATTACHED_TO_EVENT(OnGui,);
  };
};

#endif
