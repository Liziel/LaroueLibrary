#ifndef Collision_hh__
# define Collision_hh__

#include "ctvty/utils/vector3d.hh"

namespace ctvty {
  namespace component {
    class Collider;
  }
  namespace utils {
    struct Collision {
      component::Collider*			collider_from;
      component::Collider*			collider_to;
      float					force;
      ContactPoint				point;

    public:
      Collision(const Collision&) = default;
    };
  }
};

#endif
