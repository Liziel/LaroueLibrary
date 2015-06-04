#ifndef Renderer_hh__
# define Renderer_hh__

# include <string>

# include "ctvty/utils/vector3d.hh"
# include "ctvty/utils/quaternion.hh"

# include "ctvty/rendering/model3d.hh"
# include "ctvty/rendering/camera.hh"
# include "ctvty/rendering/hud.hh"
# include "ctvty/rendering/texture.hh"
# include "ctvty/rendering/shader.hh"

namespace ctvty {
  namespace rendering {

    class Renderer {
    public:
      class Implementation {
      public:
	virtual ~Implementation() {}

      public:
	virtual Model3D*		Load3DModel(const std::string&) = 0;
	virtual Texture*		LoadTexture(const std::string&) = 0;
	
      public:
	virtual void			Initialize(std::size_t width,
						   std::size_t height,
						   const std::string& window_name) = 0;

	virtual void			Update() = 0;

      public:
	virtual Camera*			CreateCamera() = 0;
	virtual std::size_t		RegisteredCameras() = 0;

      public:
	virtual std::shared_ptr<Hud>	CreateHud() = 0;

      public:
	virtual void			UseShaderAtNextDraw(std::shared_ptr<Shader>) = 0;
	virtual std::shared_ptr<Shader>	CreateShader(const std::string& vertex_p,
						     const std::string& fragment_p) = 0;

      public:
	virtual void			Pre3DRendering(int camera_id = -1) = 0;
	virtual void			PreHUDRendering(int camera_id) = 0;
	virtual void			MainHUDRendering() = 0;
	virtual void			Flush() = 0;

	virtual void			Quit() = 0;

      public:
	virtual void			SetDefaultCameraPosition(const ctvty::utils::Vector3D& position,
								 const ctvty::utils::Vector3D& lookAt,
								 const ctvty::utils::Quaternion& rotation
								 = ctvty::utils::Quaternion::identity) = 0;
      };

    public:
      static Implementation&		GetRenderer();
    };
  };
};

#endif
