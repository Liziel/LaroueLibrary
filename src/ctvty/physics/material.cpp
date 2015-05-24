#include "ctvty/physics/material.hh"

REGISTER_FOR_SERIALIZATION(ctvty::physics, Material);

namespace ctvty {
  namespace physics {
    
    Material::	Material()
      : bounciness(0), staticFriction(0), dynamicFriction(0) {}

    Material::	Material(const serialization::Archive& __serial) {
      __serial["bounciness"] & bounciness;
      __serial["staticFriction"] & staticFriction;
      __serial["dynamicFriction"] & dynamicFriction;
    }

    void	Material::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::physics::Material, __serial_instance);
      __serial["bounciness"] & bounciness;
      __serial["staticFriction"] & staticFriction;
      __serial["dynamicFriction"] & dynamicFriction;
    }
    
  };
};
