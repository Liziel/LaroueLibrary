#ifndef Faces_hh__
# define Faces_hh__

# include <vector>

# include "ctvty/utils/vector3d.hh"
# include "ctvty/utils/contactpoint.hh"
# include "std/optional.hpp"

namespace ctvty {
  namespace utils {

    class Face {
    private:
      std::vector<Vector3D>		dots;
      std::vector<Vector3D>		repered_dots;

    public:
      Vector3D				normal;
      Vector3D				x, y;

    public:
      ctvstd::Optional<ContactPoint>	Intersection(const Vector3D& position, const Vector3D& direction,
						     float& force) const;

    public:
      Vector3D				GetNormal() const;
      Vector3D				GetRelativePosition(const Vector3D& point3D) const;

    public:
      bool				IsPointInside(const Vector3D& point) const;
      ctvstd::Optional<float>		GetCollisionDistance(const Vector3D& position,
							     const Vector3D& direction) const;
      
    public:
      Face(std::vector<Vector3D>&&);
    };
  };
};

#endif
