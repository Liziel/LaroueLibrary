#include "ctvty/utils/quaternion.hh"

namespace ctvty {
  namespace utils {

    REGISTER_FOR_SERIALIZATION(Quaternion);

    Quaternion::	Quaternion(float x, float y, float z, float _scale)
      : rotation(new Vector3D(x, y, z)), scale(_scale) { }

    Quaternion::	Quaternion(const Vector3D& _rotation, float _scale)
      : rotation(new Vector3D(_rotation)), scale(_scale) { }

    Quaternion::	Quaternion(const Quaternion& _oth)
      : Quaternion(*(_oth.rotation), _oth.scale) {}

    Quaternion::	Quaternion(const serialization::Archive& __serial) {
      if (__serial.exist("scale"))
	__serial["scale"] & scale;
      else
	scale = 1.f;

      __serial["rotation"] & rotation;
    }

    void		Quaternion::Serialize(serialization::Archive& __serial_instance) {
      SERIALIZE_OBJECT_AS(ctvty::utils::Quaternion, __serial_instance);
      __serial["rotation"] & rotation;
      __serial["scale"] & scale;
    }

  };
};
