#ifndef GdlImpl_Shader_hh__
# define GdlImpl_Shader_hh__

# include "BasicShader.hh"

# include "ctvty/utils/vector3d.hh"
# include "ctvty/utils/quaternion.hh"

# include "ctvty/rendering/shader.hh"

namespace GdlImpl{
  class Renderer;
  class Shader : public ctvty::rendering::Shader {
  private:
    Renderer&		_renderer;
    gdl::BasicShader	_shader;

  public:
    gdl::BasicShader&	GetShader() { return _shader; }

  public:
    ~Shader();
    Shader(Renderer&, const std::string&, const std::string&);

  public:
    void	SetUniform(const std::string& uniform, const ctvty::utils::Quaternion& value) final;
  };
};

#endif
