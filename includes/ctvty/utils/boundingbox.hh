#ifndef BoundingBox_hh__
# define BoundingBox_hh__

# include "ctvty/utils/vector3d.hh"
# include "ctvty/utils/face.hh"

namespace ctvty {
  namespace utils {
    class BoundingBox3D {
    private:
      std::array<Face, 6>		edges;
      std::array<utils::Vector3D, 8>	vertices;

    public:
      utils::Vector3D&			operator[](int);
      void				Include(const BoundingBox3D&);

    public:
      bool				Intersect(const BoundingBox3D& box);
      BoundingBox3D			operator + (const utils::Vector3D&);

    public:
      BoundingBox3D(std::array<utils::Vector3D, 8>&&);
      BoundingBox3D();
    };
  };
};

#endif
