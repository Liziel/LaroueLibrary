#ifndef GdlRenderer_hh__
# define GdlRenderer_hh__

/* Gdl Includes */
# include "SdlContext.hh"
# include "BasicShader.hh"

# include "ctvty/rendering/renderer.hh"

# include "gdlimpl/model3d.hh"

namespace GdlImpl {
  
  class Renderer : public gdl::SdlContext, public ctvty::rendering::Renderer::Implementation {
  private:
    gdl::BasicShader			_shader;
  public:
    inline
    gdl::BasicShader&			GetShader() { return _shader; }

  private:
    std::size_t				width, height;

  private:
    ctvty::utils::Vector3D		camera_position, camera_lookAt, camera_up;

  public:
    ctvty::rendering::Model3D*		Load3DModel(const std::string&) final;

  public:
    void				Initialize(std::size_t width,
						   std::size_t height,
						   const std::string& window_name) final;
    void				Update() final;

    void				Pre3DRendering() final;
    void				PreHUDRendering() final;
    void				Flush() final;

    void				Quit() final;

  public:
    void				SetCameraPosition(const ctvty::utils::Vector3D& position,
							  const ctvty::utils::Vector3D& lookAt,
							  const ctvty::utils::Quaternion& eulerRotation) final;
  };

};

#endif
