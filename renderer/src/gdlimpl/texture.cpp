#include "gdlimpl/texture.hh"

namespace GdlImpl {

  Texture::Texture(const std::string& path) {
    texture.load(path);
  }

  void		Texture::Bind() {
    texture.bind();
  }
};
