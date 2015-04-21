#ifndef Collision_hh__
# define Collision_hh__

#include "ctvty/utils/vector3d.hh"

namespace ctvty {
  namespace component {
    class Collider;
  }
  namespace utils {
    struct Collision {
      component::Collider*	collider;
      float			force;
      ContactPoint		point;
    };
  }
};

#endif
