#include "ctvty/component/rigidbody.hh"
#include "ctvty/component/collider.hh"
#include "ctvty/world.hh"
#include "ctvty/event/clock.hh"
#include "ctvty/utils/collision.hh"
#include "ctvty/component/transform.hh"
#include "ctvty/debug.hpp"
#include "ctvty/input.hh"

REGISTER_FOR_SERIALIZATION(ctvty::component, RigidBody);

namespace ctvty {
  namespace component {
    
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

    void		RigidBody::OnDestroy() {
      for (auto collider : colliders_collisions)
	const_cast<Collider*>(collider)->Collideds().remove_if([this] (RigidBody* r) { return r == this; });
      for (auto collider : colliders_trigger)
	const_cast<Collider*>(collider)->Triggereds().remove_if([this] (RigidBody* r) { return r == this; });
    }

    void		RigidBody::FixedUpdate() {
      if (!isKinematic)
	GetVelocity() += (utils::Vector3D::down * World::gravity)
	  * event::Clock::GetClock().GetFixedDeltaTime();
      utils::Vector3D movement = GetVelocity();

      movement *= event::Clock::GetClock().GetFixedDeltaTime();
      if (!DiscreteCheckMovement(movement))
	EndMovement(movement);
    }

    bool		RigidBody::DiscreteCheckMovement(utils::Vector3D& momentum)  {
      std::list<Collider*>	colliders
	= Object::FindObjectsOfType<Collider>(); colliders.remove_if([=](Collider* collider) {
	    return collider->GetGameObject() == gameObject
	      || collider->IsTrigger()
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
	if ((rhs.Intersect(endBox) || endBox.Intersect(rhs))
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
	float		staticFriction = 0;

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
	  if ((normal * utils::Vector3D::up).DotProduct(momentum) > 0)
	    normal.y  = -normal.y;
	  if ((normal * utils::Vector3D::right).DotProduct(momentum) > 0)
	    normal.x  = -normal.x;
	  if ((normal * utils::Vector3D::forward).DotProduct(momentum) > 0)
	    normal.z  = -normal.z;

	  float bouncing = 0;
	  for (const Collider* collider_to : collision.collider_to) {
	    cumulated.collider_to.push_back(collider_to);
	    if (collider_to->GetRigidBody() == this) continue;
	    if (collider_to->GetRigidBody())
	      const_cast<RigidBody*>(collider_to->GetRigidBody())->GetVelocity() +=
		GetVelocity() / collider_to->GetRigidBody()->mass;
	    dynamicFriction += collider_to->GetMaterial().dynamicFriction;
	    staticFriction += collider_to->GetMaterial().staticFriction;
	    bouncing += collider_to->GetMaterial().bounciness;
	    GetVelocity() += -GetVelocity().Project(normal);
	  }
	  staticFriction += collision.collider_from->GetMaterial().staticFriction;
	  dynamicFriction += collision.collider_from->GetMaterial().dynamicFriction;
	  bouncing += collision.collider_from->GetMaterial().bounciness;
	  bounce += bouncing * -(GetVelocity()).Project(normal);
	  collisionNormal = (collisionNormal + normal).GetNormalized();
	  if (collision.collider_from->GetRigidBody() && collision.collider_from->GetRigidBody() != this)
	    const_cast<RigidBody*>(collision.collider_from->GetRigidBody())->GetVelocity() +=
	      GetVelocity() / collision.collider_from->GetRigidBody()->mass;
	  GetVelocity() += -GetVelocity().Project(normal);
	  cumulated.collider_to.push_back(collision.collider_from);
	}
	cumulated.collider_to.sort();
	cumulated.collider_to.unique();
	if (!isKinematic) {
	  if (dynamicFriction)
	    GetVelocity() -= -GetVelocity().ProjectOnPlane(collisionNormal) * dynamicFriction;
	  GetVelocity() += bounce;
	}
	momentum =
	  (momentum - penetration);
	position +=
	  momentum;
	cumulated.point.normal = collisionNormal;
	momentum = GetVelocity() * event::Clock::GetClock().GetFixedDeltaTime();
	if (GetVelocity().GetMagnitude() > 0.1 && DiscreteCheckMovement(momentum))
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
	  const_cast<Collider*>(c)->Collideds().remove_if([this] (RigidBody* r) { return r == this; });
	}
	colliders_collisions.clear();
	last_collision = ctvstd::none;
      }
      DetectTriggers();
    }

    void			RigidBody::EndMovement(utils::Vector3D& movement,
						       const utils::Collision& collision) {
      last_collision = new utils::Collision(collision);
      transform->GetPosition() += movement;

      std::list<const Collider*> collidings = collision.collider_to;

      colliders_collisions.remove_if([this, &collidings, &collision](const Collider* cc) {
	  for (auto c : collidings)
	    if (c == cc)
	      return false;
	  cc->BroadcastMessage("OnCollisionExit", &collision);
	  BroadcastMessage("OnCollisionExit", &collision);
	  const_cast<Collider*>(cc)->Collideds().remove_if([this] (RigidBody* r) { return r == this; });
	  return true;
	});

      collidings.remove_if([this, &collision]( const Collider* c ) {
	  for (const Collider* cc : sub_colliders)
	    if (c == cc)
	      return true;

	  for (const Collider* const cc: colliders_collisions)
	    if (c == cc)
	      return true;

	  const_cast<Collider*>(c)->Collideds().push_back(this);
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
      DetectTriggers();
    }

    void			RigidBody::DetectTriggers() {
      std::list<Collider*>	triggers
	= Object::FindObjectsOfType<Collider>(); triggers.remove_if([=](Collider* collider) {
	    return collider->GetGameObject() == gameObject
	      || !collider->IsTrigger()
	      || !collider->GetGameObject()->IsActive();
	  });

      utils::BoundingBox3D			box  = boundingBox + (transform->GetPosition());
      for (auto trigger = triggers.begin(); trigger != triggers.end();) {
	utils::BoundingBox3D rhs = (*trigger)->GetBoundingBox()
	  + (*trigger)->GetGameObject()->GetTransformation()->GetPosition();
	if (!rhs.Intersect(box) && !box.Intersect(rhs))
	  trigger = triggers.erase(trigger);
	else
	  ++ trigger;
      }

      colliders_trigger.remove_if([this, &triggers](const Collider* _trigger) {
	  for (auto trigger = triggers.begin(); trigger != triggers.end(); ++trigger) {
	    if (*trigger == _trigger) {
	      for (Collider* triggered : sub_colliders)
		_trigger->BroadcastMessage("OnTriggerStay", static_cast<const component::Collider*>(triggered));
	      BroadcastMessage("OnTriggernStay", static_cast<const component::Collider*>(_trigger));
	      trigger = triggers.erase(trigger);
	      return false;
	    }
	  }
	  for (Collider* triggered : sub_colliders)
	    _trigger->BroadcastMessage("OnTriggerExit", static_cast<const component::Collider*>(triggered));
	  BroadcastMessage("OnTriggernExit", static_cast<const component::Collider*>(_trigger));
	  const_cast<Collider*>(_trigger)->Triggereds().remove_if([this] (RigidBody* r) { return r == this; });
	  return true;
	});
      
      for (Collider* trigger : triggers) {
	for (Collider* triggered : sub_colliders)
	  trigger->BroadcastMessage("OnTriggerEnter", static_cast<const component::Collider*>(triggered));
	BroadcastMessage("OnTriggerEnter", static_cast<const component::Collider*>(trigger));
	trigger->Triggereds().push_back(this);
	colliders_trigger.push_back(trigger);
      }

    }

  };
};
