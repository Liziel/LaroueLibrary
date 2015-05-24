#include "ctvty/assets/texture.hh"
#include "ctvty/rendering/renderer.hh"
#include "ctvty/application.hh"

REGISTER_FOR_SERIALIZATION(ctvty::asset, Texture);
REGISTER_FOR_SERIALIZATION(ctvty::asset::Texture, Wrapper);

namespace ctvty {
  namespace asset {

    Texture::		Texture(const serialization::Archive& archive) {
      archive["wrapper"] & wrapper;
    }

    void		Texture::Serialize(serialization::Archive& archive_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::asset::Texture, archive_instance)
      __serial["wrapper"] & wrapper;
      if (_object)
	ctvty::Application::Assets().GetAsset(wrapper).Save(_object);
    }

    void		Texture::delayedInstantiation() {
      _object = ctvty::Application::Assets().GetAsset(wrapper).LoadAs<Wrapper>();
    }


    Texture::Wrapper::	Wrapper(const serialization::Archive& __serial) {
      __serial["path"] & path;
      model.reset(ctvty::rendering::Renderer::GetRenderer().LoadTexture(path));
    }

    void		Texture::Wrapper::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::asset::Texture::Wrapper, __serial_instance);
      __serial["path"] & path;
    }
    
  };
};
