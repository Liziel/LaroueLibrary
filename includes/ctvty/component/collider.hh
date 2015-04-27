#ifndef Collider_hh__
# define Collider_hh__

# include "ctvty/component/rigidbody.hh"
# include "ctvty/utils/collision.hh"
# include "ctvty/physics/material.hh"

namespace ctvty {
  namespace component {

    class Collider : public MonoBehaviour<Collider> {
    protected:
      utils::BoundingBox3D		boundingBox;
      RigidBody*			rigidBody;
      bool				isTrigger;
      physics::Material*		material;

    public:
      bool				IsTrigger();
      RigidBody*			GetRigidBody();
      utils::BoundingBox3D&		GetBoundingBox();
      const physics::Material*		GetMaterial();

    public:
      virtual const std::list<utils::Vector3D>&
					GetVertices() const = 0;

    protected:
      virtual ctvstd::Optional<utils::Collision>
					CollisionImpl(const Collider* contact_colliders,
						      const utils::Vector3D& position,
						      const utils::Vector3D& direction) = 0;
    public:
      ctvstd::Optional<utils::Collision>
					Collision(const std::list<Collider*>& contact_colliders,
						  const utils::Vector3D& position,
						  const utils::Vector3D& direction) {
	return ctvstd::none;
      }

    protected:
      Collider(GameObject* parent, const std::string&);
    };
    
  };
};

#endif
