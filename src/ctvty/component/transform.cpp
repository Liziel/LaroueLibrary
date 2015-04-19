#include "ctvty/component/transform.hh"


namespace ctvty {
  namespace component {

    REGISTER_FOR_SERIALIZATION(Transform);

    Transform::		Transform(GameObject* gameObject, 
				  utils::Vector3D* p , utils::Quaternion* r, utils::Vector3D* s)
      : MonoBehaviour<Transform>(gameObject, "Transform"),
	scale(s), position(p), rotation(r) { }

    Transform::		Transform(GameObject* gameObject, 
				  const utils::Vector3D& p , const utils::Quaternion& r, const utils::Vector3D& s)
      : MonoBehaviour<Transform>(gameObject, "Transform"),
	scale(new utils::Vector3D(s)), position(new utils::Vector3D(p)), rotation(new utils::Quaternion(r)) { }

    Transform::		Transform(const serialization::Archive& __serial)
      : MonoBehaviour<Transform>(nullptr, "Transform"),
	scale(nullptr), position(nullptr), rotation(nullptr) {
      if (__serial.exist("scale"))
	__serial["scale"] & scale;
      else
	scale = new utils::Vector3D(1.f, 1.f, 1.f);
      __serial["position"] & position;
      __serial["rotation"] & rotation;
    }

    void		Transform::Serialize(serialization::Archive& __serial_instance) {
      SERIALIZE_OBJECT_AS(ctvty::component::Transform, __serial_instance);
      __serial["scale"] & scale;
      __serial["position"] & position;
      __serial["rotation"] & rotation;
    }

    Object*		Transform::clone() const {
      return new Transform(nullptr, *position, *rotation, *scale);
    }

  };
};
