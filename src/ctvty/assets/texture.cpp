#include "ctvty/assets/texture.hh"
#include "ctvty/rendering/renderer.hh"

REGISTER_FOR_SERIALIZATION(ctvty::asset, Texture);

namespace ctvty {
  namespace asset {

    Texture::		Texture(const serialization::Archive& __serial) {
      __serial["path"] & path;
    }

    void		Texture::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::asset::Texture, __serial_instance);
      __serial["path"] & path;
    }

    void		Texture::delayedInstantiation() {
      texture.reset(ctvty::rendering::Renderer::GetRenderer().LoadTexture(path));
    }
  };
};
