#ifndef Model3d_hh__
# define Model3d_hh__

namespace ctvty {
  namespace rendering {

    class Model3D {
    public:
      virtual void		CreateAnimation(const std::string& name, int FrameStart, int FrameEnd) = 0;
      virtual void		PauseAnimation() = 0;
      virtual void		SetAnimation(const std::string& name, bool loop = true) = 0;

    public:
      virtual void		Draw(const ctvty::utils::Vector3D& position,
				     const ctvty::utils::Quaternion& rotation,
				     double time = ctvty::utils::event::GetClock().GetFixedDeltaTime()) = 0;
    };

  };
};


#endif
