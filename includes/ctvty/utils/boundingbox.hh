#ifndef BoundingBox_hh__
# define BoundingBox_hh__

# include "ctvty/utils/vector3d.hh"
# include "ctvty/utils/face.hh"

namespace ctvty {
  namespace utils {
    class BoundingBox3D {
    private:
      utils::Vector3D		base_vertex;
      utils::Vector3D		end_point;

    public:
      void				Include(const BoundingBox3D& box);
      void				Include(const utils::Vector3D& vertex);

    public:
      bool				Intersect(const BoundingBox3D& box);
      bool				Intersect(const utils::Vector3D& vertex);

    public:
      BoundingBox3D			operator + (const utils::Vector3D&) const;
      BoundingBox3D			operator * (float) const;

    public:
      BoundingBox3D(const utils::Vector3D&, const utils::Vector3D&);
      BoundingBox3D();
    };

    inline
    BoundingBox3D			operator * (float f, BoundingBox3D b) {
      return b * f;
    }
    
  };
};

#endif
