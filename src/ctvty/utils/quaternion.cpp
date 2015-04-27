#include <cmath>
#include "ctvty/utils/quaternion.hh"

namespace ctvty {
  namespace utils {

    REGISTER_FOR_SERIALIZATION(ctvty::utils, Quaternion);

    Quaternion::	Quaternion(float _x, float _y, float _z, float _w)
      : x(_x), y(_y), z(_z), w(_w) { }

    Quaternion::	Quaternion(const Vector3D& r, float _w)
      : Quaternion(r.x, r.y, r.z, _w) { }

    Quaternion::	Quaternion(const serialization::Archive& __serial) {
      __serial["x"] & x;
      __serial["y"] & y;
      __serial["z"] & z;
      __serial["w"] & w;
    }

    Quaternion::	~Quaternion() { }

    const Quaternion	Quaternion::identity(0., 0., 0., 1.);

    void		Quaternion::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::utils::Quaternion, __serial_instance);
      __serial["x"] & x;
      __serial["y"] & y;
      __serial["z"] & z;
      __serial["w"] & w;
    }


    const Vector3D	Quaternion::Complex() const {
      return { x, y, z };
    }

    float		Quaternion::Real() const {
      return w;
    }


    float		Quaternion::Magnitude() const {
      return std::sqrt(x*x + y*y + z*z + w*w);
    }

    const Quaternion	Quaternion::Euler(const Vector3D& v) {
      const float c3 = std::cos(v.x * 0.5f);
      const float c2 = std::cos(v.y * 0.5f);
      const float c1 = std::cos(v.z * 0.5f);
      const float s3 = std::sin(v.x * 0.5f);
      const float s2 = std::sin(v.y * 0.5f);
      const float s1 = std::sin(v.z * 0.5f);
      return {
	c1 * c2 * s3 - s1 * s2 * c3,
	c1 * s2 * c3 + s1 * c2 * s3,
	s1 * c2 * c3 - c1 * s2 * s3,
	c1 * c2 * c3 + s1 * s2 * s3
      };
    }

    const Vector3D	Quaternion::Euler() const {
      const static float	quart = 3.14159265358979323846 * 0.5;
      Vector3D			euler;
      float			sqw, sqx, sqy, sqz;

      sqw = w * w;
      sqx = x * x;
      sqy = y * y;
      sqz = z * z;
      
      euler.y = std::asin(2.f * (w * y - x * z));
      if (quart - fabs(euler.y)
	  > std::numeric_limits<float>::epsilon()) {
        euler.z = std::atan2(2.0 * (x * y + w * z),
			     sqx - sqy - sqz + sqw);
        euler.x = std::atan2(2.0 * (w * x + y * z),
			     sqw - sqx - sqy + sqz);
      } else {
        euler.z = atan2(2.f * y * z - 2.f * x * w,
			2.f * x * z + 2.f * y * w);
        euler.x = 0.f;
        
        // If facing down, reverse yaw
        if (euler.y < 0)
          euler.z = quart - euler.z;
      }
      return euler;
    }

    const Quaternion	Quaternion::Conjugate() const {
      return { -x, -y, -z, w };
    }


    const Quaternion	Quaternion::operator * (const Quaternion& rhs) const {
      return {y * rhs.z - z * rhs.y + x * rhs.w + w * rhs.x,
	  z * rhs.x - x * rhs.z + y * rhs.w + w * rhs.y,
	  x * rhs.y - y * rhs.x + z * rhs.w + w * rhs.z,
	  w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z};
    }

    const Quaternion	Quaternion::operator * (float f) const {
      return {x * f, y * f, z * f, w * f};
    }

    const Vector3D	Quaternion::RotatedVector(const Vector3D& v) const{
      const Quaternion& _this = * this;
      return ((v * _this) * Conjugate()).Complex();
    }

    const Vector3D	Quaternion::RotatedAround(const Vector3D& center, const Vector3D& p) const {
      return RotatedVector(p - center) + center;
    }
  };
};
