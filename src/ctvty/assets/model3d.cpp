#include "ctvty/assets/model3d.hh"
#include "ctvty/rendering/renderer.hh"

REGISTER_FOR_SERIALIZATION(ctvty::asset, Model3D);

namespace ctvty {
  namespace asset {
    
    Model3D::		Model3D(const serialization::Archive& archive) {
      archive["path"] & path;
    }

    void		Model3D::Serialize(serialization::Archive& archive_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::asset::Model3D, archive_instance)
      __serial["path"] & path;
    }

    void		Model3D::delayedInstantiation() {
      model.reset(ctvty::rendering::Renderer::GetRenderer().Load3DModel(path));
    }

  };
};
