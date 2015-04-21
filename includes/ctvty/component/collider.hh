#ifndef Collider_hh__
# define Collider_hh__

# include "ctvty/component/rigidbody.hh"
# include "ctvty/utils/collision.hh"
# include "ctvty/physics/material.hh"

namespace ctvty {
  namespace component {

    class Collider : public MonoBehaviour<Collider> {
      friend class RigidBody;
    private:
      utils::BoundingBox3D		boundingBox;
      RigidBody*			rigidBody;
      bool				isTrigger;
      physics::Material*		material;

    public:
      ctvstd::Optional<utils::Collision>
					Collision(const std::list<utils::Vector3D>& vertices,
						  const utils::Vector3D& position,
						  const utils::Vector3D& direction);

    };
    
  };
};

#endif
