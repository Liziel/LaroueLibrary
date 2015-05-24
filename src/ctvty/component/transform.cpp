#include "ctvty/component/transform.hh"
#include "ctvty/utils/vector3d.hh"

REGISTER_FOR_SERIALIZATION(ctvty::component, Transform);

namespace ctvty {
  namespace component {

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
	scale.reset(new utils::Vector3D(1.f, 1.f, 1.f));
      __serial["position"] & position;
      __serial["rotation"] & rotation;
    }

    void		Transform::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::component::Transform, __serial_instance);
      __serial["scale"] & scale;
      __serial["position"] & position;
      __serial["rotation"] & rotation;
    }

    Object*		Transform::clone() const {
      return new Transform(nullptr, *position, *rotation, *scale);
    }

    void		Transform::SetParent(Transform* t) {
      parent = t;
    }
    
    void		Transform::Translate(const utils::Vector3D& t) {
      *position + t;
    }

    void		Transform::EulerRotate(const utils::Vector3D& t) {
      *rotation = utils::Quaternion::Euler(t);
    }

    void		Transform::SetEulerRotation(const utils::Vector3D& t) {
      *rotation = utils::Quaternion::Euler(t);
    }

    void		Transform::Rotate(const utils::Quaternion& q) {
      *rotation = *rotation * q;
    }


    inline
    const utils::Vector3D&	Transform::GetPosition() const {
      return *position;
    }

    inline
    const utils::Quaternion&	Transform::GetRotation() const {
      return *rotation;
    }

    inline
    const utils::Vector3D&	Transform::GetScale() const {
      return *scale;
    }

    utils::Vector3D&		Transform::GetPosition() {
      return const_cast<utils::Vector3D&>(static_cast<const Transform&>(*this).GetPosition());
    }

    utils::Quaternion&		Transform::GetRotation() {
      return const_cast<utils::Quaternion&>(static_cast<const Transform&>(*this).GetRotation());
    }

    utils::Vector3D&		Transform::GetScale() {
      return const_cast<utils::Vector3D&>(static_cast<const Transform&>(*this).GetScale());
    }

    const utils::Vector3D	Transform::GetRealPosition() const {
      return (parent ? parent->GetRealPosition() : utils::Vector3D::zero) + *position;
    }

    const utils::Quaternion	Transform::GetRealRotation() const {
      return (parent ? parent->GetRealRotation() : utils::Quaternion::identity) * *rotation;
    }
  };
};
