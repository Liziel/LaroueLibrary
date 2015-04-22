#include "ctvty/component/rigidbody.hh"
#include "ctvty/component/collider.hh"
#include "ctvty/world.hh"
#include "ctvty/event/clock.hh"
#include "ctvty/utils/collision.hh"
#include "ctvty/component/transform.hh"

namespace ctvty {
  namespace component {

    REGISTER_FOR_SERIALIZATION(ctvty::component, RigidBody);

    
    RigidBody::		RigidBody(GameObject* parent)
      : MonoBehaviour<RigidBody>(parent, "RigidBody"),
	detectionMode(CollisionDetectionMode::Discrete), detectCollision(true), isKinematic(false), mass(1.0),
	velocity(new utils::Vector3D(0.f, 0.f, 0.f)), asleep(true) { }

    RigidBody::		RigidBody(const serialization::Archive& __serial)
      : MonoBehaviour<RigidBody>(nullptr, "RigidBody") {
      __serial["detectionMode"] & detectionMode;
      __serial["detectCollision"] & detectCollision;
      __serial["isKinematic"] & isKinematic;
      __serial["mass"] & mass;
      __serial["velocity"] & velocity;
    }

    
    void		RigidBody::Serialize(serialization::Archive& __serial_instance) {
      SERIALIZE_OBJECT_AS(ctvty::component::RigidBody, __serial_instance);
      __serial["detectionMode"] & detectionMode;
      __serial["detectCollision"] & detectCollision;
      __serial["isKinematic"] & isKinematic;
      __serial["mass"] & mass;
      __serial["velocity"] & velocity;
    }

    Object*		RigidBody::clone() const {
      RigidBody* _clone = new RigidBody(*this);

      _clone->velocity = new utils::Vector3D(*(this->velocity));
      return _clone;
    }

    utils::Vector3D&	RigidBody::GetVelocity() {
      return *velocity;
    }

    void		RigidBody::FixedUpdate() {
      utils::Vector3D movement = GetVelocity() + (utils::Vector3D::down * World::gravity);
      movement *= event::Clock::GetClock().GetFixedDeltaTime();
      GetVelocity() = movement / event::Clock::GetClock().GetFixedDeltaTime();
    }

    void		RigidBody::DiscreteCheckMovement(utils::Vector3D& movement)  {
      std::list<Collider*>	triggers
	= Object::FindObjectsOfType<Collider>(); triggers.remove_if([=](Collider* collider) {
	    return collider->gameObject == gameObject
	      || collider->isTrigger == false
	      || !collider->gameObject->IsActive();
	  });
      std::list<Collider*>	colliders
	= Object::FindObjectsOfType<Collider>(); colliders.remove_if([=](Collider* collider) {
	    return collider->gameObject == gameObject
	      || collider->rigidBody != nullptr
	      || !collider->gameObject->IsActive();
	  });
      std::list<RigidBody*>	rigidbodies
	= Object::FindObjectsOfType<RigidBody>(); rigidbodies.remove_if([=](RigidBody* rigidbody) {
	    return rigidbody->gameObject == gameObject
	      || !rigidbody->gameObject->IsActive();
	  });

      utils::Vector3D&				position =
	gameObject->GetTransformation()->GetPosition();
      utils::BoundingBox3D			endBox  = boundingBox + (movement + position);

      ctvstd::Optional<utils::Collision>	collision;
      float					force = 0;

      std::list< std::pair<utils::Collision, Collider*> >	future_collider_collisions;
      std::list<Collider*>					future_colliders_trigger;

      for (Collider* collider : colliders)
	if (collider->boundingBox.Intersect(endBox)
	    && (collision = collider->Collision(vertices, position, movement))) {

	  if (collision->force == movement.GetMagnitude()) {
	  } else if (force < collision->force) {
	    force = (collision)->force;
	    future_collider_collisions.clear();
	    future_collider_collisions
	      .emplace_back(*collision, collider);
	  } else if (force == (collision)->force) {
	    future_collider_collisions
	      .emplace_back(*collision, collider);
	  }

	}

      for (RigidBody* rigidbody : rigidbodies)
	for (Collider* collider : rigidbody->sub_colliders)
	  if (collider->boundingBox.Intersect(endBox)
	      && (collision = collider->Collision(vertices, position, movement))) {

	    if (collision->force == movement.GetMagnitude()) {
	    } else if (force < collision->force) {
	      force = (collision)->force;
	      future_collider_collisions.clear();
	      future_collider_collisions
		.emplace_back(*collision, collider);
	    } else if (force == (collision)->force) {
	      future_collider_collisions
		.emplace_back(*collision, collider);
	    }

	  }

      /*
       * for every collider retrived :
       *  modify velocity
       *  broadcast message
       */
      if (movement.GetMagnitude() != force) {
	utils::Vector3D	penetration = movement.GetNormalized() * force;
	utils::Vector3D	bounce = utils::Vector3D::zero;
	utils::Vector3D	slide = penetration;
	for (std::pair<utils::Collision, Collider*> pair : future_collider_collisions) {
	  if (pair.second->material->dynamicFriction)
	  slide = slide.ProjectOnPlane(pair.first.point.normal)
	    * (1 / pair.second->material->dynamicFriction);
	  if (pair.first.force != 0)
	    bounce += penetration.Reflect(pair.first.point.normal)
	      * pair.second->material->bounciness;
	  //have to broadcast to the other one too
	  BroadcastMessage("OnCollisionEnter", pair.second);
	}
	movement = bounce + slide;
	DiscreteCheckMovement(movement);
	return ;
      }

      position += movement;

      for (Collider* trigger : triggers)
	if (trigger->boundingBox.Intersect(endBox)) {
	  BroadcastMessage("onTriggerEnter", trigger);
	  future_colliders_trigger.push_back(trigger);
	}
	
    }

  };
};
