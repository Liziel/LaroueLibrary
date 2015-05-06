#include <iostream>
#include "ctvty/utils/face.hh"
#include "ctvty/debug.hpp"

namespace ctvty {
  namespace utils {

    Face::Face(std::vector<Vector3D>&& _dots)
      : dots(_dots) {
      x = (dots[1] - dots[0]).GetNormalized();
      normal = x
	.CrossProduct(dots[2] - dots[0])
	.GetNormalized();
      if (dots.size() == 4)
	y = (dots[3] - dots[0]);
      else
	y = dots[0] - x.CrossProduct(normal);

      for (const Vector3D& dot : dots)
	repered_dots.push_back(GetRelativePosition(dot));
    }

    Vector3D		Face::GetNormal() const {
      return normal;
    }

    ctvstd::Optional<ContactPoint>
    Face::Intersection(const Vector3D& position, const Vector3D& direction, float& force) const {
      ctvstd::Optional<float>	dForce;

      ctvty::debug::CompressedLogs("Normal At Face", normal, "Colliding Point", position, "Direction", direction);
      if (!(dForce = Face::GetCollisionDistance(position, direction)))
	return ctvstd::none;
      ctvty::debug::Logs("collision Force = ", *dForce);
      if (*dForce > 1 || *dForce < 0)
	return ctvstd::none;
      if (!IsPointInside(position + direction * *dForce))
	return ctvstd::none;
      force = 1. - *dForce;
      std::cerr << "valid" << std::endl;
      return {position + direction * *dForce, normal};
    }

    ctvstd::Optional<float>
			Face::GetCollisionDistance(const Vector3D& position, const Vector3D& direction) const {
      if (!normal.DotProduct(direction))
	return ctvstd::none;
      float  k = -(normal.DotProduct(position) - normal.DotProduct(dots[0])) /
	(normal.DotProduct(direction));
      return k;
    }

    Vector3D		Face::GetRelativePosition(const Vector3D& vector) const {
      return Vector3D(vector.ScalarProjection(x), vector.ScalarProjection(y), 0.f);
    }

    bool		Face::IsPointInside(const Vector3D& point) const {
      if (!((dots[0] - point).CrossProduct(x).GetNormalized() == normal
	    || (dots[0] - point).CrossProduct(x).GetNormalized() == -normal))
	return false;
      Vector3D	relative = GetRelativePosition(point);

      ctvty::debug::CompressedLogs("relative", relative, "relatives", repered_dots, "dots", dots);
      std::size_t it, itp;
      bool retval = false;
      for (it = 0, itp = repered_dots.size() - 1; it < repered_dots.size(); itp = it++) {
	if ( (repered_dots[it].y > relative.y) != (repered_dots[itp].y > relative.y) &&
	     (relative.x
	      < (repered_dots[itp].x - repered_dots[it].x) * (relative.y - repered_dots[it].y)
	      / (repered_dots[itp].y - repered_dots[it].y) + repered_dots[it].x) )
	  retval = !retval;
     }
      return retval;
    }
  };
};
