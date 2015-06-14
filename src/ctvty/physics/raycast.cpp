#include <limits>
#include "ctvty/physics/raycast.hh"
#include "ctvty/debug.hpp"

namespace ctvty {
  namespace physics {
    ctvstd::Optional<Raycaster::Hit>
			Raycaster::Raycast(const utils::Vector3D& origin,
					   const utils::Vector3D& direction,
					   float maxDistance,
					   std::function<bool(component::Collider*)> selector
					   ) {
      ctvstd::Optional<Hit>		hit(ctvstd::none);
      std::list<component::Collider*> colliders = Object::FindObjectsOfType<component::Collider>();
      ctvty::debug::Log(origin);
      colliders.remove_if([selector] (component::Collider* collider) {
	  return !collider->GetGameObject()->IsActive() ||
	    !selector(collider);
	});

      float distance = maxDistance;
      for (component::Collider* collider : colliders) {
	ctvstd::Optional<float> result = collider->RayCollision(origin, direction);
	
	if (result && *result < distance && *result >= 0) {
	  distance = *result;
	  hit = Raycaster::Hit{collider, distance};
	}
      }
      return hit;
    }
  };
};
