#include "ctvty/utils/boundingbox.hh"

namespace ctvty {
  namespace utils {

    BoundingBox3D::BoundingBox3D()
      : BoundingBox3D(
		      { Vector3D::zero, Vector3D::zero, Vector3D::zero, Vector3D::zero,
			  Vector3D::zero, Vector3D::zero, Vector3D::zero, Vector3D::zero}) {}

  };
};
