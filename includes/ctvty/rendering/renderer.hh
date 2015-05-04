#ifndef Renderer_hh__
# define Renderer_hh__

# include <string>

# include "ctvty/utils/vector3d.hh"
# include "ctvty/utils/quaternion.hh"

# include "ctvty/rendering/model3d.hh"

namespace ctvty {
  namespace rendering {

    class Renderer {
    public:
      class Implementation {
      public:
	virtual Model3D*		Load3DModel(const std::string&) = 0;

      public:
	virtual void			Initialize(std::size_t width,
						   std::size_t height,
						   const std::string& window_name) = 0;

	virtual void			Update() = 0;

	virtual void			Pre3DRendering() = 0;
	virtual void			PreHUDRendering() = 0;
	virtual void			Flush() = 0;

	virtual void			Quit() = 0;

      public:
	virtual void			SetCameraPosition(const ctvty::utils::Vector3D& position,
							  const ctvty::utils::Vector3D& lookAt,
							  const ctvty::utils::Vector3D& eulerRotation
							  = ctvty::utils::Vector3D::zero) = 0;
      };

    public:
      static Implementation&		GetRenderer();

    public:
      static void			Initialize(std::size_t width,
						   std::size_t height,
						   const std::string& window_name);
      static void			Update();
      static void			Flush();
      static void			Quit();

    public:
      static void			SetCameraPosition(const ctvty::utils::Vector3D& position,
							  const ctvty::utils::Quaternion& rotation);
    };
  };
};

#endif
