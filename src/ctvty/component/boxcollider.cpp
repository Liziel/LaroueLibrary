#include "ctvty/component/boxcollider.hh"

namespace ctvty{
  namespace component {
    REGISTER_FOR_SERIALIZATION(ctvty::component, BoxCollider);
    
    BoxCollider::		BoxCollider(GameObject* gameObject)
      : Collider(gameObject, "BoxCollider"),
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

      __serial["material"] & Collider::material;
      __serial["isTrigger"] & Collider::isTrigger;

      CalculateVertices();
      for (utils::Vector3D& vertex : vertices)
	boundingBox.Include(vertex);
    }

    void			BoxCollider::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(BoxCollider::Serialize, __serial_instance);
      __serial["+size"] & psize;
      __serial["-size"] & msize;
      __serial["scale"] & scale;

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
    }

    const std::list<utils::Vector3D>&
				BoxCollider::GetVertices() const {
      return vertices;
    }

    void			BoxCollider::Scale(float f) {
      boundingBox = boundingBox * f;
      *msize * f;
      *psize * f;
      for (utils::Vector3D& vertex : vertices)
	vertex * f;
    }

    ctvstd::Optional<utils::Collision> 
				BoxCollider::CollisionImpl(const Collider* contact_colliders,
						     const utils::Vector3D& position,
						     const utils::Vector3D& direction) {
      return ctvstd::none;
    }


  };
};
