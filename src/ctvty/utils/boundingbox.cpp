#include "ctvty/utils/boundingbox.hh"
#include "ctvty/debug.hpp"

namespace ctvty {
  namespace utils {

    /*
     * 3 Vertices
     * Bot.[x,y,z]
     */

    BoundingBox3D::	BoundingBox3D(const utils::Vector3D& _base_vertex, const utils::Vector3D& _end_point)
      : base_vertex(_base_vertex), end_point(_end_point) {}

    BoundingBox3D::	BoundingBox3D()
      : BoundingBox3D( Vector3D::zero, Vector3D::zero ) {}

    void		BoundingBox3D::Serialize(serialization::Archive& __serial) const {
      __serial["base_vertex"] & &base_vertex;
      __serial["end_point"] & &end_point;
    }

    void		BoundingBox3D::Include(const BoundingBox3D& box) {
      Include(Vector3D(box.base_vertex.x, box.base_vertex.y, box.base_vertex.z));

      Include(Vector3D(box.end_point.x, box.base_vertex.y, box.base_vertex.z));
      Include(Vector3D(box.base_vertex.x, box.end_point.y, box.base_vertex.z));
      Include(Vector3D(box.base_vertex.x, box.base_vertex.y, box.end_point.z));

      Include(Vector3D(box.end_point.x, box.end_point.y, box.base_vertex.z));
      Include(Vector3D(box.base_vertex.x, box.end_point.y, box.end_point.z));
      Include(Vector3D(box.end_point.x, box.base_vertex.y, box.end_point.z));

      Include(Vector3D(box.end_point.x, box.end_point.y, box.end_point.z));
    }

    void		BoundingBox3D::Include(const Vector3D& vertex) {
      if (vertex.y < base_vertex.y)
	base_vertex.y = vertex.y;
      if (vertex.y > end_point.y)
	end_point.y = vertex.y;

      if (vertex.x < base_vertex.x)
	base_vertex.x = vertex.x;
      if (vertex.x > end_point.x)
	end_point.x = vertex.x;

      if (vertex.z < base_vertex.z)
	base_vertex.z = vertex.z;
      if (vertex.z > end_point.z)
	end_point.z = vertex.z;
    }

    bool		BoundingBox3D::Intersect(const BoundingBox3D& box) const {
      return (
	      Intersect(Vector3D(box.base_vertex.x, box.base_vertex.y, box.base_vertex.z))	||

	      Intersect(Vector3D(box.end_point.x, box.base_vertex.y, box.base_vertex.z))	||
	      Intersect(Vector3D(box.base_vertex.x, box.end_point.y, box.base_vertex.z))	||
	      Intersect(Vector3D(box.base_vertex.x, box.base_vertex.y, box.end_point.z))	||

	      Intersect(Vector3D(box.end_point.x, box.end_point.y, box.base_vertex.z))	||
	      Intersect(Vector3D(box.base_vertex.x, box.end_point.y, box.end_point.z))	||
	      Intersect(Vector3D(box.end_point.x, box.base_vertex.y, box.end_point.z))	||

	      Intersect(Vector3D(box.end_point.x, box.end_point.y, box.end_point.z))	      
	      );
    }

    bool		BoundingBox3D::Intersect(const utils::Vector3D& vertex) const {
      ctvty::debug::CompressedLogs("BoundingVertex", vertex);
      return (
	      base_vertex	.x	< vertex.x	&&
	      end_point		.x	> vertex.x	&&

	      base_vertex	.y	< vertex.y	&&
	      end_point		.y	> vertex.y	&&

	      base_vertex	.z	< vertex.z	&&
	      end_point		.z	> vertex.z	);

    }

    BoundingBox3D	BoundingBox3D::operator + (const utils::Vector3D& vector) const {
      return BoundingBox3D(base_vertex + vector, end_point + vector);
    }


    BoundingBox3D	BoundingBox3D::operator * (float f) const {
      return BoundingBox3D(base_vertex * f, end_point * f);
    }
  };
};
