#include "ctvty/component/boxcollider.hh"

namespace ctvty{
  namespace component {
    REGISTER_FOR_SERIALIZATION(ctvty::component, BoxCollider);
    
    /*
     * F* Y* I'm too powerfull to have a parent!
     */
    BoxCollider::		BoxCollider(utils::Vector3D* b, utils::Vector3D* s, float sc)
      : Collider(nullptr, "BoxCollider"), base_offset(b), size(s), scale(sc) {}

    BoxCollider::		BoxCollider(const utils::Vector3D& b, const utils::Vector3D& s, float sc)
      : BoxCollider(new utils::Vector3D (b), new utils::Vector3D (s), sc) {}



    BoxCollider::		BoxCollider(const serialization::Archive& __serial)
      : BoxCollider(__serial["base_offset"].as<utils::Vector3D*>(),
		    __serial["size"].as<utils::Vector3D*>(),
		    __serial.exist("scale") ? __serial["scale"].as<float>() : 1.f) {}
  };
};
