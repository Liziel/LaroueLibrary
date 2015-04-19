#include <iostream>
#include "ctvty/utils/vector3d.hh"

namespace ctvty {
  namespace utils {

    REGISTER_FOR_SERIALIZATION(Vector3D);

    Vector3D::		Vector3D(float _x, float _y, float _z, float _scale)
      : x(_x), y(_y), z(_z), scale(_scale) { }

    Vector3D::		Vector3D(const serialization::Archive& __serial) {
      if (__serial.exist("scale"))
	__serial["scale"] & scale;
      else
	scale = 1.f;

      __serial["x"] & x;
      __serial["y"] & y;
      __serial["z"] & z;
    }

    void		Vector3D::Serialize(serialization::Archive& __serial_instance) {
      SERIALIZE_OBJECT_AS(ctvty::utils::Vector3D, __serial_instance);
      __serial["x"] & x;
      __serial["y"] & y;
      __serial["z"] & z;
      __serial["scale"] & scale;
    }

  };
};
