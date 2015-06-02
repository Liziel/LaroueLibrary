#ifndef RayCast_hh__
# define RayCast_hh__

# include <limits>

# include "std/optional.hpp"

# include "ctvty/component/collider.hh"

# include "ctvty/utils/vector3d.hh"

namespace ctvty {
  namespace physics {

    class Raycaster {
    public:
      struct Hit {
	component::Collider*	collider;
	float		dist;
      };
    public:
      static ctvstd::Optional<Hit>	Raycast(const utils::Vector3D& origin,
						const utils::Vector3D& direction,
						float maxDistance = std::numeric_limits<float>::infinity(),
						std::function<bool(component::Collider*)> selector
						= [](component::Collider*) -> bool {
						  return true;
						});
    };

  };
};

#endif
