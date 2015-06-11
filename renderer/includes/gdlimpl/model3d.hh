#ifndef GdlModel3D_hh__
# define GdlModel3D_hh__

# include <memory>

# include "Model.hh"
# include "Geometry.hh"

# include "ctvty/rendering/model3d.hh"

namespace GdlImpl {

  class Renderer;
  class Model3D : public ctvty::rendering::Model3D {
  private:
    std::string			path;
    bool			loadStatus;
    std::unique_ptr<gdl::Model>		model;
    std::unique_ptr<gdl::Geometry>	cube;

  public:
    Renderer&			renderer;

  public:
    Model3D(Renderer&);
    Model3D(Renderer&, const std::string&);

  public:
    void		CreateAnimation(const std::string& name,
					int FrameStart, int FrameEnd) final;
    void		PauseAnimation() final;
    void		SetAnimation(const std::string& name, bool loop = true) final;
    float		GetFrameDuration() final;

  public:
    void		Draw(const ctvty::utils::Vector3D& position,
			     const ctvty::utils::Vector3D& scale,
			     const ctvty::utils::Quaternion& rotation,
			     double time = ctvty::event::Clock::GetClock().GetFixedDeltaTime()) final;
  };

};

#endif
