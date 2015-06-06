#include "ctvty/assets/shader.hh"


REGISTER_FOR_SERIALIZATION(ctvty::asset, Shader);
REGISTER_FOR_SERIALIZATION(ctvty::asset::Shader, Wrapper);

namespace ctvty {
  namespace assst {

    Shader::		Shader(const serialization::Archive& __serial) {
      __serial["wrapper"]	& wrapper;
    }

    void		Model3D::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::asset::Shader);
      __serial["wrapper"]	& wrapper;      
    }

    void		Model3D::delayedInstantiation() {
      _object = ctvty::Application::Assets().GetAsset(wrapper).LoadAs<Wrapper>();
      if (_object == nullptr)
	std::cerr << "Shader Couldn't be instantiated" << std::endl;
    }

    Wrapper::Shader::		Shader(const serialization::Archive& __serial) {
      __shader["fragment shader"] & fragment_shader;
      __shader["vertex shader"] & vertex_shader;
      shader.reset(ctvty::rendering::Renderer::GetRenderer().CreateShader(vertex_shader, fragment_shader));
    }

    void		Shader::Wrapper::Serialize(serialization::Archive& __serial_instance) {
      SERIALIZE_OBJECT_AS(ctvty::asset::Shader::Wrapper);
      __shader["fragment shader"] & fragment_shader;
      __shader["vertex shader"] & vertex_shader;
    }
  }
};
