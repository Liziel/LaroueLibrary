#ifndef Renderer_hh__
# define Renderer_hh__

# include "model3d.hh"

namespace ctvty {
  namespace rendering {

    class Renderer {
    private:
      static RendererImpl*&			GetRenderer();

    public:
      class RendererImpl {
      public:
	virtual Model3D*		Load3DModel(const std::string&) = 0;

      public:
	virtual void			Initialize(std::size_t width,
						   std::size_t height,
						   const std::string& window_name) = 0;
	virtual void			Update() = 0;
	virtual void			Flush() = 0;
	virtual void			Quit() = 0;

      public:
	virtual void			SetCameraPosition(const ctvty::utils::Vector3D& position,
							  const ctvty::utils::Vector3D& rotation) = 0;
      };


    public:
      static void			LoadRenderingLibrary(const std::string& path);

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
