#include <list>
#include <future>
#include "ctvty/component/boxcollider.hh"
#include "ctvty/component/transform.hh"

namespace ctvty{
  namespace component {
    REGISTER_FOR_SERIALIZATION(ctvty::component, BoxCollider);
    
    BoxCollider::		BoxCollider(GameObject* gameObject)
      : Collider(gameObject, "BoxCollider"),
	offset(new utils::Vector3D(utils::Vector3D::zero)),
	msize(new utils::Vector3D(utils::Vector3D::zero)),
	psize(new utils::Vector3D(utils::Vector3D::zero)), scale(0.) {}

    Object*			BoxCollider::clone() const {
      serialization::Serial	copy;
      BoxCollider* _clone = nullptr;
      std::shared_ptr<BoxCollider> b;

      copy & const_cast<BoxCollider*>(this);
      static_cast<const serialization::Serial&>(copy) & _clone;

      return _clone;
    }

    BoxCollider::		BoxCollider(const serialization::Archive& __serial)
      : BoxCollider(nullptr) {
      __serial["+size"] & psize;
      __serial["-size"] & msize;

      if (__serial.exist("scale"))
	__serial["scale"] & scale;
      else
	scale = 1.f;

      if (__serial.exist("offset"))
	__serial["offset"] & offset;
      else
	offset .reset(new utils::Vector3D(utils::Vector3D::zero));

      __serial["material"] & Collider::material;
      __serial["isTrigger"] & Collider::isTrigger;

      CalculateVertices();

      for (utils::Vector3D& vertex : vertices)
	boundingBox.Include(vertex);
    }

    void			BoxCollider::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::component::BoxCollider, __serial_instance);
      __serial["+size"] & psize;
      __serial["-size"] & msize;
      __serial["scale"] & scale;
      __serial["offset"] & offset;

      __serial["material"] & Collider::material;
      __serial["isTrigger"] & Collider::isTrigger;
    }

    void			BoxCollider::CalculateVertices() {
      vertices = {
	(utils::Vector3D(psize->x,	psize->y,	psize->z)	* scale),
	(utils::Vector3D(-msize->x,	psize->y,	psize->z)	* scale),
	(utils::Vector3D(psize->x,	-msize->y,	psize->z)	* scale),
	(utils::Vector3D(-msize->x,	-msize->y,	psize->z)	* scale),

	(utils::Vector3D(psize->x,	psize->y,	-msize->z)	* scale),
	(utils::Vector3D(-msize->x,	psize->y,	-msize->z)	* scale),
	(utils::Vector3D(psize->x,	-msize->y,	-msize->z)	* scale),
	(utils::Vector3D(-msize->x,	-msize->y,	-msize->z)	* scale)
      };
      faces[0].reset(new utils::Face({
	(utils::Vector3D(-msize->x,	-msize->y,	-msize->z)	* scale),
	(utils::Vector3D(-msize->x,	psize->y,	-msize->z)	* scale),
	(utils::Vector3D(-msize->x,	psize->y,	psize->z)	* scale),
	(utils::Vector3D(-msize->x,	-msize->y,	psize->z)	* scale)
	}));
      faces[1].reset(new utils::Face({
	(utils::Vector3D(psize->x,	-msize->y,	-msize->z)	* scale),
	(utils::Vector3D(psize->x,	psize->y,	-msize->z)	* scale),
	(utils::Vector3D(psize->x,	psize->y,	psize->z)	* scale),
	(utils::Vector3D(psize->x,	-msize->y,	psize->z)	* scale)
	}));
      faces[2].reset(new utils::Face({
	(utils::Vector3D(-msize->x,	-msize->y,	-msize->z)	* scale),
	(utils::Vector3D(psize->x,	-msize->y,	-msize->z)	* scale),
	(utils::Vector3D(psize->x,	-msize->y,	psize->z)	* scale),
	(utils::Vector3D(-msize->x,	-msize->y,	psize->z)	* scale)
	}));
      faces[3].reset(new utils::Face({
	(utils::Vector3D(-msize->x,	psize->y,	-msize->z)	* scale),
	(utils::Vector3D(psize->x,	psize->y,	-msize->z)	* scale),
	(utils::Vector3D(psize->x,	psize->y,	psize->z)	* scale),
	(utils::Vector3D(-msize->x,	psize->y,	psize->z)	* scale)
	}));
      faces[4].reset(new utils::Face({
	(utils::Vector3D(psize->x,	psize->y,	-msize->z)	* scale),
	(utils::Vector3D(-msize->x,	psize->y,	-msize->z)	* scale),
	(utils::Vector3D(-msize->x,	-msize->y,	-msize->z)	* scale),
	(utils::Vector3D(psize->x,	-msize->y,	-msize->z)	* scale)
	}));
      faces[5].reset(new utils::Face({
	(utils::Vector3D(psize->x,	psize->y,	psize->z)	* scale),
	(utils::Vector3D(-msize->x,	psize->y,	psize->z)	* scale),
	(utils::Vector3D(-msize->x,	-msize->y,	psize->z)	* scale),
	(utils::Vector3D(psize->x,	-msize->y,	psize->z)	* scale)
	}));
    }

    const std::list<utils::Vector3D>&
				BoxCollider::GetVertices() const {
      return vertices;
    }

    void			BoxCollider::Scale(float f) {
      boundingBox = boundingBox * f;
      *msize * f;
      *psize * f;
      CalculateVertices();
    }

    void	list_predecl() {std::list<ctvty::component::Collider const*> merde;}
    
    ctvstd::Optional<utils::Collision>
    BoxCollider::CollisionImpact(const utils::Vector3D& v, const utils::Vector3D& d) const {
      utils::Vector3D vertex(transform->GetRotation().RotatedVector(v - transform->GetPosition() - *offset));
      utils::Vector3D direction(transform->GetRotation().RotatedVector(d));
      ctvstd::Optional<utils::Collision>	collision(ctvstd::none);
      float					force = 0;

      if (vertex.x <= -msize->x) {
	float	_force = 0;
	ctvstd::Optional<utils::ContactPoint> contact =
	    faces[0]->Intersection(vertex, direction, _force);
	if (contact && _force > force) {
	  contact->point  = (transform->GetRotation().Inverse()).RotatedVector(contact->point
								    + transform->GetPosition() + *offset);
	  contact->normal = (transform->GetRotation().Inverse()).RotatedVector(contact->normal);
	  collision = new utils::Collision{nullptr, {}, force = _force, *contact};
	}
      } else if (vertex.x >= psize->x) {
	float	_force = 0;
	ctvstd::Optional<utils::ContactPoint> contact =
	    faces[1]->Intersection(vertex, direction, _force);
	if (contact && _force > force) {
	  contact->point  = (transform->GetRotation().Inverse()).RotatedVector(contact->point
								    + transform->GetPosition() + *offset);
	  contact->normal = (transform->GetRotation().Inverse()).RotatedVector(contact->normal);
	  collision = new utils::Collision{nullptr, {}, force = _force, *contact};
	}
      }

      if (vertex.y <= -msize->y) {
	float	_force = 0;
	ctvstd::Optional<utils::ContactPoint> contact =
	    faces[2]->Intersection(vertex, direction, _force);
	if (contact && _force > force) {
	  contact->point  = (transform->GetRotation().Inverse()).RotatedVector(contact->point
								    + transform->GetPosition() + *offset);
	  contact->normal = (transform->GetRotation().Inverse()).RotatedVector(contact->normal);
	  collision = new utils::Collision{nullptr, {}, force = _force, *contact};
	}
      } else if (vertex.y >= psize->y) {
	float	_force = 0;
	ctvstd::Optional<utils::ContactPoint> contact =
	    faces[3]->Intersection(vertex, direction, _force);
	if (contact && _force > force) {
	  contact->point  = (transform->GetRotation().Inverse()).RotatedVector(contact->point
								    + transform->GetPosition() + *offset);
	  contact->normal = (transform->GetRotation().Inverse()).RotatedVector(contact->normal);
	  collision = new utils::Collision{nullptr, {}, force = _force, *contact};
	}
      }

      if (vertex.z <= -msize->z) {
	float	_force = 0;
	ctvstd::Optional<utils::ContactPoint> contact =
	    faces[4]->Intersection(vertex, direction, _force);
	if (contact && _force > force) {
	  contact->point  = (transform->GetRotation().Inverse()).RotatedVector(contact->point
								    + transform->GetPosition() + *offset);
	  contact->normal = (transform->GetRotation().Inverse()).RotatedVector(contact->normal);
	  collision = new utils::Collision{nullptr, {}, force = _force, *contact};
	}
      } else if (vertex.z >= psize->z) {
	float	_force = 0;
	ctvstd::Optional<utils::ContactPoint> contact =
	    faces[5]->Intersection(vertex, direction, _force);
	if (contact && _force > force) {
	  contact->point  = (transform->GetRotation().Inverse()).RotatedVector(contact->point
								    + transform->GetPosition() + *offset);
	  contact->normal = (transform->GetRotation().Inverse()).RotatedVector(contact->normal);
	  collision = new utils::Collision{nullptr, {}, force = _force, *contact};
	}
      }

      return collision;
    }

    ctvstd::Optional<utils::Collision>
    BoxCollider::CollisionIter(std::list<utils::Vector3D>::const_iterator b,
			       std::list<utils::Vector3D>::const_iterator e,
			       const utils::Vector3D& position,
			       const utils::Quaternion& rotation,
			       const utils::Vector3D& direction) const {
      long	len(std::distance(b, e));

      if (len < 10) {
	ctvstd::Optional<utils::Collision> collision;
	for (;b != e; ++b) {
	  ctvstd::Optional<utils::Collision> collided
	    = CollisionImpact(rotation.RotatedAround(utils::Vector3D::zero, *b) + position, direction);
	  if (collided) {
	    if (!collision) {
	      collision = collided;
	    } else if (collision->force < collided->force) {
	      collision = collided;
	    } else if (collision->force == collided->force) {
	      collision->point.point  = (collision->point.point  + collided->point.point ) / 2;
	      collision->point.normal = (collision->point.normal + collided->point.normal) / 2;
	    }
	  }
	}
	return collision;
      }

      std::list<utils::Vector3D>::const_iterator m = b;
      std::advance(m, len / 2);
      auto handle = std::async(std::launch::async, &BoxCollider::CollisionIter,
			       this, m, e, position, rotation, direction);
      ctvstd::Optional<utils::Collision> collision1 = BoxCollider::CollisionIter(b, m, position, rotation, direction);
      ctvstd::Optional<utils::Collision> collision2 = handle.get();
      ctvstd::Optional<utils::Collision> retval;

      if (collision1 && collision2) {
	if (collision1->force > collision2->force)
	  retval = collision1;
	else if (collision2->force > collision1->force)
	  retval = collision2;
	else {
	  retval = collision1;
	  retval->point.point = (retval->point.point + collision2->point.point) / 2;
	  retval->point.normal = (retval->point.normal + collision2->point.normal) / 2;
	}
      } else if (collision1) {
	retval = collision1;
      } else if (collision2) {
	retval = collision2;
      }
      return retval;
    }

    ctvstd::Optional<utils::Collision> 
    BoxCollider::CollisionImpl(const Collider*		contact_collider,
							   const utils::Vector3D&	position,
							   const utils::Quaternion&	rotation,
							   const utils::Vector3D&	direction) const {
      return BoxCollider::CollisionIter(contact_collider->GetVertices().begin(),
					contact_collider->GetVertices().end(),
					position, rotation, direction);
    }


  };
};
