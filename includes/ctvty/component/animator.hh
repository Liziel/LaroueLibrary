#ifndef Animator_hh__
# define Animator_hh__

# include "ctvty/monobehaviour.hpp"
# include "ctvty/component/renderer.hh"

namespace ctvty {
  namespace component {

    class Animator : public MonoBehaviour<Animator> {
    public:
      class State : public serialization::Serializable {
      private:
	std::string				name;

      private:
	bool					loop;
	std::string				next_state;
	std::map< std::string, std::string >	transitions;

      private:
	float					start_animation_key;
	float					end_animation_key;
	float					animation_speed_modifier;

      private:
	float					animation_key;

      public:
			State() = default;
			State(const serialization::Archive&);
	void		Serialize(serialization::Archive&) const override;

      public:
	bool		IsTransition(const std::string&) const;
	std::string	GetTransition(const std::string&) const;
	std::string	GetEndTransition() const;

      public:
	void		Create(Renderer*);
	void		StartPlaying();
	bool		HasStopped();
	double		GetFrame(double increment);
      };
      
    private:
      std::map< std::string, std::shared_ptr<State> >	states;
      std::string						current_state;

    public:
			Animator(const serialization::Archive&);
      void		Serialize(serialization::Archive&) const override;

    public:
      ctvty::Object*	clone() const;

    public:
      void		Awake();

    public:
      bool		Trigger(const std::string&);
      double		GetFrame();
    };
  };
};

#endif
