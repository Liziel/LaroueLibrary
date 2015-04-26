#include "ctvty/utils/contactpoint.hh"

namespace ctvty {
  namespace utils {

    ContactPoint::	ContactPoint(const Vector3D& p, const Vector3D& n)
      : point(p), normal(n) {}
  };
};
