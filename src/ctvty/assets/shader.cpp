#include "ctvty/assets/shader.hh"
#include "ctvty/application.hh"

REGISTER_FOR_SERIALIZATION(ctvty::asset, Shader);
REGISTER_FOR_SERIALIZATION(ctvty::asset::Shader, Wrapper);

namespace ctvty {
  namespace asset {

    Shader::		Shader(const serialization::Archive& __serial) {
      __serial["wrapper"]	& wrapper;
    }

    void		Shader::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::asset::Shader, __serial_instance);
      __serial["wrapper"]	& wrapper;      
    }

    void		Shader::delayedInstantiation() {
      _object = ctvty::Application::Assets().GetAsset(wrapper).LoadAs<Wrapper>();
      if (_object == nullptr)
	std::cerr << "Shader Couldn't be instantiated" << std::endl;
    }

    Shader::Wrapper::	Wrapper(const serialization::Archive& __serial) {
      __serial["fragment shader"] & fragment_shader;
      __serial["vertex shader"] & vertex_shader;
      shader = (ctvty::rendering::Renderer::GetRenderer().CreateShader(vertex_shader, fragment_shader));
    }

    void		Shader::Wrapper::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::asset::Shader::Wrapper, __serial_instance);
      __serial["fragment shader"] & fragment_shader;
      __serial["vertex shader"] & vertex_shader;
    }

  }
};
