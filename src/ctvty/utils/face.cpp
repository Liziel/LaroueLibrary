#include "ctvty/utils/face.hh"

namespace ctvty {
  namespace utils {

    Face::Face(std::vector<Vector3D>&& _dots)
      : dots(_dots) {}

    Vector3D		Face::GetNormal() const {
      return 
	(dots[1] - dots[0])
	.CrossProduct(dots[2] - dots[0])
	.GetNormalized();
    }
  };
};
