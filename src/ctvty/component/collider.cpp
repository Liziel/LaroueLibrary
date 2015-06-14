#include "ctvty/component/collider.hh"
#include "ctvty/component/transform.hh"
#include "ctvty/component/rigidbody.hh"

namespace ctvty {
  namespace component {
    Collider::Collider(GameObject* parent, const std::string& name)
      : MonoBehaviour<Collider>(parent, name), rigidBody(nullptr), material(new physics::Material) { }

    void			Collider::Awake() {
      RigidBody*	r = GetComponent<RigidBody>();

      if (r != nullptr) {
	rigidBody = r;
	r->RegisterCollider(this);
      }
      r = nullptr;
    }

    void			Collider::OnDestroy() {
      for (RigidBody* triggered : triggereds)
	triggered->ExitTrigger(this);
      for (RigidBody* collided : collideds)
	collided->ExitCollision(this);
    }

    bool			Collider::IsTrigger() const {
      return isTrigger;
    }

    const RigidBody*		Collider::GetRigidBody() const {
      return rigidBody;
    }

    const utils::BoundingBox3D&	Collider::GetBoundingBox() const {
      return boundingBox;
    }

    const physics::Material&	Collider::GetMaterial() const {
      return *material;
    }

    ctvstd::Optional<utils::Collision>
				Collider::Collision(const std::list<Collider*>& contact_colliders,
						    const utils::Vector3D&	position,
						    const utils::Quaternion&	quaternion,
						    const utils::Vector3D&	direction) const {      
      ctvstd::Optional<utils::Collision>	collision;
      for (const Collider* collider : contact_colliders) {
	ctvstd::Optional<utils::Collision>	collided;
	collided = CollisionImpl(collider, position, quaternion, direction);
	if (collided) {
	  if (!collision)
	    collision = collided;
	  else if (collision->force < collided->force)
	    collision = collided;
	  else if (collision->force == collided->force) {
	    collision->point.point  = (collision->point.point  + collided->point.point) / 2;
	    collision->point.normal = (collision->point.normal + collided->point.normal) / 2;
	  }
	  collision->collider_to.push_back(collider);
	  collision->collider_from = this;
	}

	collided
	  = collider->CollisionImpl(this, transform->GetPosition(), transform->GetRotation(), -direction);
	if (collided) {
	  if (!collision)
	    collision = collided;
	  else if (collision->force < collided->force)
	    collision = collided;
	  else if (collision->force == collided->force) {
	    collision->point.point  = (collision->point.point  + collided->point.point) / 2;
	    collision->point.normal = (collision->point.normal + collided->point.normal) / 2;
	  }
	  collision->collider_to.push_back(collider);
	  collision->collider_from = this;
	}

      }
      return collision;
    }


  };
};
