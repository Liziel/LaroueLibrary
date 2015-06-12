#include "ctvty/component/transform.hh"
#include "ctvty/utils/vector3d.hh"

REGISTER_FOR_SERIALIZATION(ctvty::component, Transform);

namespace ctvty {
  namespace component {

    Transform::		Transform(GameObject* gameObject, 
				  utils::Vector3D* p , utils::Quaternion* r, utils::Vector3D* s)
      : MonoBehaviour<Transform>(gameObject, "Transform"),
	scale(s), position(p), rotation(r), parent(nullptr) { }

    Transform::		Transform(GameObject* gameObject,
				  const utils::Vector3D& p , const utils::Quaternion& r, const utils::Vector3D& s)
      : MonoBehaviour<Transform>(gameObject, "Transform"),
	scale(new utils::Vector3D(s)), position(new utils::Vector3D(p)), rotation(new utils::Quaternion(r)), parent(nullptr) { }

    Transform::		Transform(const serialization::Archive& __serial)
      : MonoBehaviour<Transform>(nullptr, "Transform"),
	scale(nullptr), position(nullptr), rotation(nullptr), parent(nullptr) {
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

    void		Transform::Awake() {
      if (gameObject->HasParent())
	parent = gameObject->GetParent().GetTransformation();
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

    const utils::Vector3D	Transform::GetHierarchyPosition() const {
      return *position + (parent ? parent->GetHierarchyPosition() : ctvty::utils::Vector3D::zero);
    }

    const utils::Quaternion	Transform::GetHierarchyRotation() const {
      return *rotation * (parent ? parent->GetHierarchyRotation() : ctvty::utils::Quaternion::identity);
    }

    const utils::Vector3D	Transform::GetHierarchyScale() const {
      return *scale * (parent ? parent->GetHierarchyScale() : ctvty::utils::Vector3D::one);
    }

  };
};
