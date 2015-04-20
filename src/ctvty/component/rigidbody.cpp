#include "ctvty/component/rigidbody.hh"

namespace ctvty {
  namespace component {

    REGISTER_FOR_SERIALIZATION(ctvty::component, RigidBody);

    
    RigidBody::		RigidBody(GameObject* parent)
      : MonoBehaviour<RigidBody>(parent, "RigidBody"),
	detectionMode(discrete), detectCollision(true), isKinematic(false), mass(1.0),
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

    void		RigidBody::Start() { }
    void		RigidBody::FixedUpdate() { }

  };
};
