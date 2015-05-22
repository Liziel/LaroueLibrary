#ifndef GdlRenderer_hh__
# define GdlRenderer_hh__

/* Gdl Includes */
# include "SdlContext.hh"
# include "BasicShader.hh"

# include "ctvty/rendering/renderer.hh"

# include "gdlimpl/model3d.hh"
# include "gdlimpl/camera.hh"
# include "gdlimpl/hud.hh"

namespace GdlImpl {
  
  class Renderer : public gdl::SdlContext, public ctvty::rendering::Renderer::Implementation {
  private:
    gdl::BasicShader			_shader;
  public:
    inline
    gdl::BasicShader&			GetShader() { return _shader; }

  private:
    std::size_t				width, height;
  public:
    inline std::size_t			GetWidth() { return width; }
    inline std::size_t			GetHeight() { return height; }

  private:
    ctvty::utils::Vector3D		camera_position, camera_lookAt, camera_up;

  public:
    ctvty::rendering::Model3D*		Load3DModel(const std::string&) final;
    ctvty::rendering::Texture*		LoadTexture(const std::string&) final;

  public:
    void				Initialize(std::size_t width,
						   std::size_t height,
						   const std::string& window_name) final;
    void				Update() final;

  private:
    std::list<Camera*>			cameras;
  public:
    ctvty::rendering::Camera*		CreateCamera() final;
    void				UnregisterCamera(Camera*);
    std::size_t				RegisteredCameras() final;
    inline const std::list<Camera*>&	Cameras() { return cameras; }

  private:
    std::list<Hud*>			huds;
  public:
    ctvty::rendering::Hud*		CreateHud() final;

  public:
    void				Pre3DRendering(int camera_id) final;
    void				PreHUDRendering(int camera_id) final;
    void				MainHUDRendering() final;
    void				Flush() final;

    void				Quit() final;

  public:
    void				SetDefaultCameraPosition(const ctvty::utils::Vector3D& position,
								 const ctvty::utils::Vector3D& lookAt,
								 const ctvty::utils::Quaternion& eulerRotation) final;
  };

};

#endif
