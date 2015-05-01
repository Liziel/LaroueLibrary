#ifndef ContactPoint_hh__
# define ContactPoint_hh__

#include "ctvty/utils/vector3d.hh"

namespace ctvty {
  namespace utils {
    struct ContactPoint {
      Vector3D		point;
      Vector3D		normal;

    public:
      ContactPoint(const Vector3D& point, const Vector3D& normal);
      ContactPoint() = default;
    };
  };
};

#endif
