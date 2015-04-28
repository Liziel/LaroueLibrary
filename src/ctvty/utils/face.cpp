#include <iostream>
#include "ctvty/utils/face.hh"

namespace ctvty {
  namespace utils {

    Face::Face(std::vector<Vector3D>&& _dots)
      : dots(_dots) {
      x = (dots[1] - dots[0]).GetNormalized();
      normal = x
	.CrossProduct(dots[2] - dots[0])
	.GetNormalized();
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

      if (!(dForce = Face::GetCollisionDistance(position, direction)))
	return ctvstd::none;
      if (*dForce > 1 || *dForce < 0)
	return ctvstd::none;
      if (!IsPointInside(position + direction * *dForce))
	return ctvstd::none;
      force = 1. - *dForce;
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
      if (!(point.CrossProduct(x).GetNormalized() == normal || point.CrossProduct(x).GetNormalized() == -normal))
	return false;
      std::cout << "oui" << std::endl;
      Vector3D	relative = GetRelativePosition(point);

      std::size_t it, itp;
      bool retval = false;
      for (it = 0, itp = repered_dots.size() - 1; it < repered_dots.size(); itp = it++) {
	if (repered_dots[it] == relative)
	  return true;
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
