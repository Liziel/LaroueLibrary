#include "ctvty/event/clock.hh"
#include "ctvty/component/animator.hh"

REGISTER_FOR_SERIALIZATION(ctvty::component, Animator);
REGISTER_FOR_SERIALIZATION(ctvty::component::Animator, State);

namespace ctvty {
  namespace component {

    Animator::	Animator(const serialization::Archive& __serial)
      : MonoBehaviour(nullptr, "Animator") {
      __serial["default_state"] & current_state;
      __serial["states"] & states;
    }

    void	Animator::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::component::Animator, __serial_instance);
      __serial["default_state"] & current_state;
      __serial["states"] & states;
    }


    ctvty::Object*	Animator::clone() const {
      serialization::Serial	copy;
      copy & this;
      return copy.as<Animator*>();
    }

    void	Animator::Awake() {
      Renderer*	renderer = GetComponent<Renderer>();

      for ( typename decltype(this->states)::value_type & pair : states)
	pair.second->Create(renderer);
    }

    
    bool	Animator::Trigger(const std::string& trigger) {
      if (states[current_state]->IsTransition(trigger)) {
	current_state = states[current_state]->GetTransition(trigger);
	states[current_state]->StartPlaying();
	return true;
      }
      return false;
    }

    double	Animator::GetFrame() {
      if (states[current_state]->HasStopped())
	current_state = states[current_state]->GetEndTransition();
      return states[current_state]->GetFrame(ctvty::event::Clock::GetClock().GetFixedDeltaTime());
    }



    Animator::State::	State(const serialization::Archive& __serial) {
      __serial["name"] & name;

      loop = false;
      if (__serial.exist("next_state"))
	__serial["next_state"] & next_state;
      else
	loop = true;

      __serial["transitions"] & transitions;

      __serial["start"] & start_animation_key;
      __serial["end"] & end_animation_key;
      if (__serial.exist("speed"))
	__serial["speed"] & animation_speed_modifier;
      else
	animation_speed_modifier = 1.f;
    }

    void		Animator::State::Serialize(serialization::Archive& __serial) const {
      __serial["name"] & name;

      if (loop == false)
	__serial["next_state"] & next_state;

      __serial["transitions"] & transitions;

      __serial["start"] & start_animation_key;
      __serial["end"] & end_animation_key;
      __serial["speed"] & animation_speed_modifier;
    }


    bool		Animator::State::IsTransition(const std::string& trigger) const {
      return (transitions.find(trigger) != transitions.end());
    }

    std::string		Animator::State::GetTransition(const std::string& trigger) const {
      return (transitions.at(trigger));
    }

    std::string		Animator::State::GetEndTransition() const {
      return next_state;
    }


    void		Animator::State::Create(Renderer* r) {
      r->CreateAnimation(name, start_animation_key, end_animation_key);
    }

    void		Animator::State::StartPlaying() {
      animation_key = 0;
    }

    bool		Animator::State::HasStopped() {
      return !loop && (animation_key > end_animation_key - start_animation_key);
    }

    double		Animator::State::GetFrame(double incr) {
      ++animation_key;
      return incr * animation_speed_modifier;
    }
    
  };
};
