#include "ctvty/assets/police.hh"
#include "ctvty/application.hh"

REGISTER_FOR_SERIALIZATION(ctvty::asset, Police);
REGISTER_FOR_SERIALIZATION(ctvty::asset::Police, Wrapper);


namespace ctvty {
  namespace asset {

    Police::		Police(const serialization::Archive& archive) {
      archive["wrapper"] & wrapper;
    }

    void		Police::Serialize(serialization::Archive& archive_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::asset::Police, archive_instance);
      __serial["wrapper"] & wrapper;
    }

    void		Police::delayedInstantiation() {
      _object = ctvty::Application::Assets().GetAsset(wrapper).LoadAs<Wrapper>();
    }

    Police::Wrapper::	~Wrapper() {
      if (police)
	TTF_CloseFont(police);
    }

    Police::Wrapper::	Wrapper(const serialization::Archive& __serial) {
      __serial["size"] & size;
      __serial["path"] & path;
      police = TTF_OpenFont((ctvty::Application::Assets().GetLocation() + path.c_str)(), size);
      if (!police)
	std::cout << "error " << TTF_GetError() << std::endl;
    }

    void		Police::Wrapper::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::asset::Police::Wrapper, __serial_instance);
      __serial["size"] & size;
      __serial["path"] & path;
    }
  };
};
