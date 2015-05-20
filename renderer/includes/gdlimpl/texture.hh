#ifndef GdlTexture_hh__
# define GdlTexture_hh__

# include <string>

# include "ctvty/rendering/texture.hh"
# include "Texture.hh"

namespace GdlImpl {
  
  class Texture : public ctvty::rendering::Texture {
  private:
    gdl::Texture	texture;

  public:
    Texture(const std::string&);

  public:
    void		Bind() final;
  };
};

#endif
