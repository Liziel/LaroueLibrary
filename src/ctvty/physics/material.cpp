#include "ctvty/physics/material.hh"


namespace ctvty {
  namespace physics {
    
    void	Material::Serialize(serialization::Archive& __serial) const {
      __serial["bounciness"] & bounciness;
      __serial["staticFriction"] & staticFriction;
      __serial["dynamicFriction"] & dynamicFriction;
    }
    
  };
};
