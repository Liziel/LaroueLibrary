#ifndef _Movement_hh__
# define _Movement_hh__

# include "ctvty/monobehaviour.hpp"
# include "ctvty/component/rigidbody.hh"

namespace user_defined {
  namespace component {
    namespace player {
      class Movement : public ctvty::MonoBehaviour<Movement> {
      public:
	Movement(const serialization::Archive& __serial);
	void	Serialize(serialization::Archive&) const;

      public:
	ctvty::component::RigidBody*	rb;

      public:
	void	Awake();
	void	Update();
      };
    };
  };
};

#endif
