#include "ctvty/assets/model3d.hh"
#include "ctvty/rendering/renderer.hh"
#include "ctvty/application.hh"

REGISTER_FOR_SERIALIZATION(ctvty::asset, Model3D);
REGISTER_FOR_SERIALIZATION(ctvty::asset::Model3D, Wrapper);

namespace ctvty {
  namespace asset {
    
    Model3D::		Model3D(const serialization::Archive& archive) {
      archive["wrapper"] & wrapper;
    }

    void		Model3D::Serialize(serialization::Archive& archive_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::asset::Model3D, archive_instance)
      __serial["wrapper"] & wrapper;
      if (_object)
	ctvty::Application::Assets().GetAsset(wrapper).Save(_object);
    }

    void		Model3D::delayedInstantiation() {
      _object = ctvty::Application::Assets().GetAsset(wrapper).LoadAs<Wrapper>();
      if (_object == nullptr)
	std::cerr << "Model3d Couldn't be instantiated" << std::endl;
    }


    Model3D::Wrapper::	Wrapper(const serialization::Archive& __serial) {
      __serial["path"] & path;
      model.reset(ctvty::rendering::Renderer::GetRenderer()
		  .Load3DModel(ctvty::Application::Assets().GetLocation() + path));
    }

    void		Model3D::Wrapper::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::asset::Model3D::Wrapper, __serial_instance);
      __serial["path"] & path;
    }

  };
};
