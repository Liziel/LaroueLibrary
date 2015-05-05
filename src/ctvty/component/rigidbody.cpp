#include "ctvty/component/rigidbody.hh"
#include "ctvty/component/collider.hh"
#include "ctvty/world.hh"
#include "ctvty/event/clock.hh"
#include "ctvty/utils/collision.hh"
#include "ctvty/component/transform.hh"
#include "ctvty/debug.hpp"

namespace ctvty {
  namespace component {

    REGISTER_FOR_SERIALIZATION(ctvty::component, RigidBody);

    
    RigidBody::		RigidBody(GameObject* parent)
      : MonoBehaviour<RigidBody>(parent, "RigidBody"),
	detectionMode(CollisionDetectionMode::Discrete), detectCollision(true), isKinematic(false), mass(1.0),
	velocity(new utils::Vector3D(0.f, 0.f, 0.f)), asleep(true) {
    }

    RigidBody::		RigidBody(const serialization::Archive& __serial)
      : MonoBehaviour<RigidBody>(nullptr, "RigidBody") {
      __serial["detectionMode"] & detectionMode;
      __serial["detectCollision"] & detectCollision;
      __serial["isKinematic"] & isKinematic;
      __serial["mass"] & mass;
      __serial["velocity"] & velocity;
    }

    void		RigidBody::RegisterCollider(ctvty::component::Collider* c) {
      sub_colliders.push_back(c);
      boundingBox.Include(c->GetBoundingBox());
      ctvty::debug::Logs(c->GetBoundingBox(), boundingBox);
    }
    
    void		RigidBody::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::component::RigidBody, __serial_instance);
      __serial["detectionMode"] & detectionMode;
      __serial["detectCollision"] & detectCollision;
      __serial["isKinematic"] & isKinematic;
      __serial["mass"] & mass;
      __serial["velocity"] & velocity;
    }

    Object*		RigidBody::clone() const {
      serialization::Serial	copy;
      RigidBody* _clone = nullptr;

      copy & this;
      static_cast<const serialization::Serial&>(copy) & _clone;

      return _clone;
    }

    utils::Vector3D&	RigidBody::GetVelocity() {
      return *velocity;
    }

    void		RigidBody::FixedUpdate() {
      GetVelocity() += (utils::Vector3D::down * World::gravity)
	* event::Clock::GetClock().GetFixedDeltaTime();
      utils::Vector3D movement = GetVelocity();

      movement *= event::Clock::GetClock().GetFixedDeltaTime();
      ctvty::debug::Logs("Velocity: ", GetVelocity(), "\nPosition: ", transform->GetPosition(), "\n\n");
      if (!DiscreteCheckMovement(movement))
	EndMovement(movement);
    }

    bool		RigidBody::DiscreteCheckMovement(utils::Vector3D& momentum)  {
      std::list<Collider*>	colliders
	= Object::FindObjectsOfType<Collider>(); colliders.remove_if([=](Collider* collider) {
	    return collider->GetGameObject() == gameObject
	      || !collider->GetGameObject()->IsActive();
	  });

      if (!colliders.size())
	return false;

      utils::Vector3D&				position =
	transform->GetPosition();
      utils::BoundingBox3D			endBox  = boundingBox + (momentum + position);

      ctvstd::Optional<utils::Collision>	collision(ctvstd::none);
      float					force = 0;

      std::list< utils::Collision >		future_collider_collisions;

      for (Collider* collider : colliders) {
	if (collider->IsTrigger())
	  continue;
	utils::BoundingBox3D rhs = collider->GetBoundingBox()
	  + collider->GetGameObject()->GetTransformation()->GetPosition();
	ctvstd::Optional<utils::Collision>	_collision;
	if (rhs.Intersect(endBox))
	  ctvty::debug::CompressedLogs("intersect with", collider);
	if (rhs.Intersect(endBox)
	    && (_collision = collider->Collision(sub_colliders, position, transform->GetRotation(), momentum))) {
	  collision = _collision;
	  if (collision->force == momentum.GetMagnitude()) {
	  } else if (force < collision->force) {
	    force = (collision)->force;
	    future_collider_collisions.clear();
	    future_collider_collisions
	      .emplace_back(*collision);
	  } else if (force == (collision)->force) {
	    future_collider_collisions
	      .emplace_back(*collision);
	  }

	}
      }

      /*
       * for every collider retrived :
       *  modify velocity
       *  broadcast message
       */
      if (collision) {
	utils::Collision cumulated;
	utils::Vector3D	penetration = momentum * force;
	utils::Vector3D	bounce = utils::Vector3D::zero;
	utils::Vector3D	collisionNormal = utils::Vector3D::zero;
	float		dynamicFriction = 0;

	for (utils::Collision& collision : future_collider_collisions) {
	  cumulated.collider_to.insert(cumulated.collider_to.end(), 
				       collision.collider_to.begin(), collision.collider_to.end());
	  cumulated.collider_to.push_back(collision.collider_from);
	  cumulated.force = collision.force;
	  if (cumulated.point.point == utils::Vector3D::zero)
	    cumulated.point.point = collision.point.point;
	  else
	    cumulated.point.point = (cumulated.point.point + collision.point.point) / 2;

	  utils::Vector3D normal = collision.point.normal;
	  if (normal.DotProduct(momentum) > 0)
	    normal = -normal;

	  float bouncing = 0;
	  for (const Collider* collider_to : collision.collider_to) {
	    bouncing += collider_to->GetMaterial().bounciness;
	    dynamicFriction += collider_to->GetMaterial().dynamicFriction;
	  }
	  dynamicFriction += collision.collider_from->GetMaterial().dynamicFriction;
	  bouncing += collision.collider_from->GetMaterial().bounciness;
	  bounce += bouncing * -(GetVelocity()).Project(normal);
	  
	  collisionNormal = (collisionNormal + normal).GetNormalized();
	}
	ctvty::debug::CompressedLogs("projection on Plane with normal", collisionNormal,
				     " of", momentum,
				     " is ", -momentum.ProjectOnPlane(collisionNormal));
	ctvty::debug::CompressedLogs("cumalated contact point", cumulated.point.point);
	ctvty::debug::CompressedLogs("pre-momentum", momentum);
	ctvty::debug::CompressedLogs("bounciness", bounce);
	GetVelocity() += -GetVelocity().Project(collisionNormal);
	if (dynamicFriction)
	  GetVelocity() -= -GetVelocity().ProjectOnPlane(collisionNormal) * dynamicFriction;
	GetVelocity() += bounce;
	momentum =
	  (momentum - penetration);
	position += 
	  momentum;
	cumulated.point.normal = collisionNormal;
	ctvty::debug::CompressedLogs("after-momentum", GetVelocity() * event::Clock::GetClock().GetFixedDeltaTime());
	momentum = GetVelocity() * event::Clock::GetClock().GetFixedDeltaTime();
	if (DiscreteCheckMovement(momentum))
	  return true;
	else {
	  EndMovement(momentum, cumulated);
	  return true;
	}
      }
      return false;	
    }


    void			RigidBody::EndMovement(utils::Vector3D& movement) {
      transform->GetPosition() += movement;
      GetVelocity() = movement / event::Clock::GetClock().GetFixedDeltaTime();

      if (last_collision) {
	for (const Collider* c : colliders_collisions) {
	  c->BroadcastMessage("OnCollisionExit", static_cast<const utils::Collision*>(&*last_collision));
	  BroadcastMessage("OnCollisionExit", static_cast<const utils::Collision*>(&*last_collision));
	}
	colliders_collisions.clear();
	last_collision = ctvstd::none;
      }
    }

    void			RigidBody::EndMovement(utils::Vector3D& movement,
						       const utils::Collision& collision) {
      last_collision = new utils::Collision(collision);
      transform->GetPosition() += movement;
      ctvty::debug::Logs("Velocity: ", GetVelocity(), "\nPosition: ", transform->GetPosition(), "\n\n");

      std::list<const Collider*> collidings = collision.collider_to;

      colliders_collisions.remove_if([this, &collidings, &collision](const Collider* cc) {
	  for (auto c : collidings)
	    if (c == cc)
	      return false;
	  cc->BroadcastMessage("OnCollisionExit", &collision);
	  BroadcastMessage("OnCollisionExit", &collision);
	  return true;
	});

      collidings.remove_if([this, &collision]( const Collider* c ) {
	  for (const Collider* cc : sub_colliders)
	    if (c == cc)
	      return true;

	  for (const Collider* const cc: colliders_collisions)
	    if (c == cc)
	      return true;

	  if (c->GetRigidBody() != NULL)
	    std::cout << "Collided with a rigidBody" << std::endl;
	  c->BroadcastMessage("OnCollisionEnter", static_cast<const utils::Collision*>(&collision));
	  BroadcastMessage("OnCollisionEnter", static_cast<const utils::Collision*>(&collision));
	  return false;
	});

      for (const Collider* c : colliders_collisions) {
	c->BroadcastMessage("OnCollisionStay", &collision);
	BroadcastMessage("OnCollisionStay", &collision);
      }
      colliders_collisions.insert(colliders_collisions.end(),
				collidings.begin(), collidings.end());
    }

  };
};
