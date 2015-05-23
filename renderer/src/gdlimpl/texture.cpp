#include <iostream>

#include "gdlimpl/texture.hh"

namespace GdlImpl {

  Texture::Texture(const std::string& path) {
    if (!texture.load(path)) {
      std::cout << "invalid" << std::endl;
    }
  }

  void		Texture::Bind() {
    texture.bind();
  }
};
