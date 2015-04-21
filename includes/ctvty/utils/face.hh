#ifndef Faces_hh__
# define Faces_hh__

#include "ctvty/utils/vector3d.hh"
#include "ctvty/utils/contactpoint.hh"
#include "std/optional.hpp"

namespace ctvty {
  namespace utils {
    class Face {
    private:
      std::vector<Vector3D>		dots;
      Vector3D				normal;

    public:
      ctvstd::Optional<ContactPoint>	Intersection(const Vector3D& position, const Vector3D& direction);
    };
  };
};

#endif
