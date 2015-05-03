#ifndef GdlRenderer_hh__
# define GdlRenderer_hh__

# include <SdlContext.hh>

# include "ctvty/rendering/renderer.hh"

namespace GdlImpl {
  
  class Renderer : public gdl::SdlContext, public ctvty::rendering::Renderer::Implementation {
  public:
    ctvty::rendering::Model3D*		Load3DModel(const std::string&) final;

  public:
    void				Initialize(std::size_t width,
						   std::size_t height,
						   const std::string& window_name) final;
    void				Update() final;
    void				Flush() final;
    void				Quit() final;

  public:
    void				SetCameraPosition(const ctvty::utils::Vector3D& position,
							  const ctvty::utils::Vector3D& rotation) final;
  };

};

#endif
