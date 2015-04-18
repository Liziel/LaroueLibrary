#ifndef MonoBehaviour_hpp__
# define MonoBehaviour_hpp__

/* AND HERE IS THE MONSTER !! */

# include "ctvty/behaviour.hh"
# include "event/event/delayed.hh"

# define ATTACHED_TO_EVENT(__event__, __parameters__)			\
  template<typename type>						\
  auto attach_to_event ## __event__ (type* _this)			\
  -> decltype( _this -> __event__ ( __parameters__ ), void )		\
  { this->Component::RegisterListener(## __event__ ,			\
				      &type :: __event__); }		\
									\
  void attach_to_event ## __event__ ( ... )				\
  { /* nop */ }								\
  

# define ATTACH_THIS_TO(__event__)					\
  attach_to_event ## __event__ <child_class*>(dynamic_cast<child_class*>(this))	\

namespace ctvty {
  class Collision;
  class Collision2D;

  template <typename child_class>
  class MonoBehaviour : public Behaviour {
  private:
    /*
     *Invoke -> do delayed call
     */
    std::list< event::DelayedActionContainer< std::list<DelayedActionContainer*> >* >	invoked;

  public:
    template<typename ... arguments>
    void			Invoke(void (child_class::*invokable)(arguments...), float time, arguments ...) {
      invoked.push_back(new DelayedActionContainer(invoked,
						   std::bind(invokable,
							     dynamic_cast<child_class*>(this),
							     arguments ... ),
						   time)
			);
    }

    template<typename ... arguments>
    void			InvokeRepeating(void (child_class::*invokable)(arguments...),
						float time, float repeat,
						arguments ...) {
      invoked.push_back(new DelayedActionContainer(invoked,
						   std::bind(invokable,
							     dynamic_cast<child_class*>(this),
							     arguments ... ),
						   time, repeat)
			);
    }

    void			CancelInvoke() {
      for (auto invoking : invoked)
	delete invoking;
      invoked.clear();
    }


  public:
    MonoBehaviour(GameObject* parent, const std::string& name)
      : Behaviour(parent, name) {
      ATTACH_THIS_TO(Awake);
      ATTACH_THIS_TO(Start);
      ATTACH_THIS_TO(Update);
      ATTACH_THIS_TO(FixedUpdate);
      ATTACH_THIS_TO(OnApplicationQuit);
      ATTACH_THIS_TO(OnBecomeInvisible);
      ATTACH_THIS_TO(OnBecomeVisible);
      ATTACH_THIS_TO(OnCollisionEnter);
      ATTACH_THIS_TO(OnCollisionEnter2D);
      ATTACH_THIS_TO(OnCollisionExit);
      ATTACH_THIS_TO(OnCollisionExit2D);
      ATTACH_THIS_TO(OnCollisionStay);
      ATTACH_THIS_TO(OnCollisionStay2D);
      ATTACH_THIS_TO(OnTriggerEnter);
      ATTACH_THIS_TO(OnTriggerEnter2D);
      ATTACH_THIS_TO(OnTriggerExit);
      ATTACH_THIS_TO(OnTriggerExit2D);
      ATTACH_THIS_TO(OnTriggerStay);
      ATTACH_THIS_TO(OnTriggerStay2D);
      ATTACH_THIS_TO(OnDestroy);
      ATTACH_THIS_TO(OnDisable);
      ATTACH_THIS_TO(OnEnable);
      ATTACH_THIS_TO(OnGui);
    }

  private:
    ATTACHED_TO_EVENT(Awake,);
    ATTACHED_TO_EVENT(Start,);
    ATTACHED_TO_EVENT(Update,);
    ATTACHED_TO_EVENT(FixedUpdate,);
    ATTACHED_TO_EVENT(OnApplicationQuit,);
    ATTACHED_TO_EVENT(OnBecomeInvisible,);
    ATTACHED_TO_EVENT(OnBecomeVisible,);
    ATTACHED_TO_EVENT(OnCollisionEnter,		new const Collision);
    ATTACHED_TO_EVENT(OnCollisionEnter2D,	new const Collision2D);
    ATTACHED_TO_EVENT(OnCollisionExit,		new const Collision);
    ATTACHED_TO_EVENT(OnCollisionExit2D,	new const Collision2D);
    ATTACHED_TO_EVENT(OnCollisionStay,		new const Collision);
    ATTACHED_TO_EVENT(OnCollisionStay2D,	new const Collision2D);
    ATTACHED_TO_EVENT(OnTriggerEnter,		new const Collision);
    ATTACHED_TO_EVENT(OnTriggerEnter2D,		new const Collision2D);
    ATTACHED_TO_EVENT(OnTriggerExit,		new const Collision);
    ATTACHED_TO_EVENT(OnTriggerExit2D,		new const Collision2D);
    ATTACHED_TO_EVENT(OnTriggerStay,		new const Collision);
    ATTACHED_TO_EVENT(OnTriggerStay2D,		new const Collision2D);
    ATTACHED_TO_EVENT(OnDestroy,);
    ATTACHED_TO_EVENT(OnDisable,);
    ATTACHED_TO_EVENT(OnEnable,);
    ATTACHED_TO_EVENT(OnGui,);
  };
};

#endif
