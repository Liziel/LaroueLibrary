#include "gdlimpl/renderer.hh"

namespace ctvty {
  namespace rendering {

    Renderer::Implementation&	Renderer::GetRenderer() {
      static ::GdlImpl::Renderer	renderer;
      return renderer;
    }
  };
};


namespace GdlImpl {
  void		Renderer::Initialize(std::size_t width,
				     std::size_t height,
				     const std::string& window_name) {
    
  }
};
