#ifndef Camera_hh__
# define Camera_hh__

# include "ctvty/utils/vector3d.hh"
# include "ctvty/utils/quaternion.hh"

namespace ctvty {
  namespace rendering {
    
    class Camera {
    public:
      virtual ~Camera() {}
    public:
      virtual void			SetCameraPosition(const ctvty::utils::Vector3D& position,
							  const ctvty::utils::Vector3D& lookAt,
							  const ctvty::utils::Quaternion& rotation
							  = ctvty::utils::Quaternion::identity) = 0;

    public:
      virtual void			SetViewPort(std::size_t baseX, std::size_t baseY,
					    std::size_t width, std::size_t height)  = 0;
      virtual void			DetectViewPort(int priority)  = 0;
    };
    
  };
};

#endif
