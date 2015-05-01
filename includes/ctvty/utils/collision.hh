#ifndef Collision_hh__
# define Collision_hh__

#include <list>
#include "ctvty/utils/vector3d.hh"
#include "ctvty/utils/contactpoint.hh"

namespace ctvty {
  namespace component {
    class Collider;
  }
  namespace utils {
    struct Collision {
      const component::Collider*			collider_from;
      std::list<const component::Collider*>		collider_to;
      float					force;
      ContactPoint				point;

    public:
      Collision(const Collision&) = default;
      Collision() = default;
    };
  }
};

#endif
