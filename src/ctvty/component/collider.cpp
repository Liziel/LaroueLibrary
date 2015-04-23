#include "ctvty/component/collider.hh"

namespace ctvty {
  namespace component {
    bool			Collider::IsTrigger() {
      return isTrigger;
    }

    RigidBody*			Collider::GetRigidBody() {
      return rigidBody;
    }

    utils::BoundingBox3D&	Collider::GetBoundingBox() {
      return boundingBox;
    }

    const physics::Material*	Collider::GetMaterial() {
      return material;
    }

  };
};
