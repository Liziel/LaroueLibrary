#include "ctvty/component/collider.hh"
#include "ctvty/component/transform.hh"

namespace ctvty {
  namespace component {
    Collider::Collider(GameObject* parent, const std::string& name)
      : MonoBehaviour<Collider>(parent, name) {}

    bool			Collider::IsTrigger() {
      return isTrigger;
    }

    RigidBody*			Collider::GetRigidBody() {
      return rigidBody;
    }

    utils::BoundingBox3D&	Collider::GetBoundingBox() {
      return boundingBox;
    }

    const physics::Material*	Collider::GetMaterial() {
      return material;
    }

    ctvstd::Optional<utils::Collision>
				Collider::Collision(const std::list<Collider*>& contact_colliders,
						    const utils::Vector3D&	position,
						    const utils::Quaternion&	quaternion,
						    const utils::Vector3D&	direction) const {
      ctvstd::Optional<utils::Collision>	collision;
      for (const Collider* collider : contact_colliders) {
	ctvstd::Optional<utils::Collision>	collided
	  = collider->CollisionImpl(collider, position, quaternion, -direction);
	if (collided) {
	  if (!collision)
	    collision = collided;
	  else if (collision->force < collided->force)
	    collision = collided;
	  else if (collision->force == collided->force) {
	    collision->point.point  = (collision->point.point  + collided->point.point) / 2;
	    collision->point.normal = (collision->point.normal + collided->point.point) / 2;
	  }
	  collision->collider_to.push_back(collider);
	  collision->collider_from = this;
	}

	collided = CollisionImpl(this,
				 collider->transform->GetPosition(),
				 collider->transform->GetRotation(),
				 direction);
	if (collided) {
	  if (!collision)
	    collision = collided;
	  else if (collision->force < collided->force)
	    collision = collided;
	  else if (collision->force == collided->force) {
	    collision->point.point  = (collision->point.point  + collided->point.point) / 2;
	    collision->point.normal = (collision->point.normal + collided->point.point) / 2;
	  }
	}

      }
    }


  };
};
