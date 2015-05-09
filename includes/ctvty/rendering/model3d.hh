#ifndef Model3d_hh__
# define Model3d_hh__

# include "ctvty/utils/vector3d.hh"
# include "ctvty/utils/quaternion.hh"

# include "ctvty/event/clock.hh"

namespace ctvty {
  namespace rendering {

    class Model3D {
    public:
      virtual void		CreateAnimation(const std::string& name, int FrameStart, int FrameEnd) = 0;
      virtual void		PauseAnimation() = 0;
      virtual void		SetAnimation(const std::string& name, bool loop = true) = 0;
      virtual float		GetFrameDuration() = 0;

    public:
      virtual void		Draw(const ctvty::utils::Vector3D& position,
				     const ctvty::utils::Vector3D& scale,
				     const ctvty::utils::Quaternion& rotation,
				     double time = ctvty::event::Clock::GetClock().GetFixedDeltaTime()) = 0;
    };

  };
};


#endif
