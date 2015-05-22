#ifndef HudRendering_hh__
# define HudRendering_hh__

# include <string>
# include "ctvty/utils/vector3d.hh"
# include "ctvty/rendering/camera.hh"
# include "ctvty/rendering/texture.hh"

namespace ctvty {
  namespace rendering {
    
    class Hud {
    public:
      virtual ~Hud() {}

    public:
      virtual void	SetTexture(std::shared_ptr<ctvty::rendering::Texture>&) = 0;
      virtual void	SetText(const std::string&) = 0;
      
    public:
      virtual void	Associate(std::shared_ptr<Camera>&) = 0;
      virtual void	SetPosition(float sizex, float sizey,
				    float offx = 0, float offy = 0) = 0;

    public:
      virtual void	SetWorldSpace(const ctvty::utils::Quaternion& rotation,
				       const ctvty::utils::Vector3D& position) = 0;
      virtual void	SetScreenSpace(int level) = 0;
    };
  };
};

#endif
