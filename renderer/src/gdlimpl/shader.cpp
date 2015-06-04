#include "gdlimpl/shader.hh"
#include "gdlimpl/renderer.hh"
namespace GdlImpl{
  Shader::		Shader(Renderer& renderer, const std::string& vertex, const std::string& fragment)
    : _renderer(renderer) {
    _shader.load(vertex, GL_VERTEX_SHADER);
    _shader.load(fragment, GL_FRAGMENT_SHADER);
  }

  Shader::		~Shader() {
    _renderer.UnsetShader(this);
  }

  void		Shader::SetUniform(const std::string& uniform, const ctvty::utils::Quaternion& value) {
    _shader
      .setUniform(uniform, glm::vec4(value.Complex().x, value.Complex().y, value.Complex().z, value.Real()));
  }
};
