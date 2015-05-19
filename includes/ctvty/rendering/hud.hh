#ifndef HudRendering_hh__
# define HudRendering_hh__

# include <string>
# include "ctvty/utils/vector3d.hh"
# include "ctvty/rendering/camera.hh"

namespace ctvty {
  namespace rendering {
    
    class Hud {
    public:
      virtual ~Hud() {}

    public:
      virtual void	SetTexture() = 0;
      virtual void	SetText(const std::string&) = 0;
      
    public:
      virtual void	Associate(Camera*) = 0;
      virtual void	SetPosition(float offx, float offy,
				    float sizex, float sizey);

    public:
      virtual void	DrawWorldSpace(const ctvty::utils::Vector3D& normal,
				       const ctvty::utils::Vector3D& position);
      virtual void	DrawScreenSpace();
    };
  };
};

#endif
