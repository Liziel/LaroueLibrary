#include <cmath>
#include "ctvty/utils/vector3d.hh"

namespace ctvty {
  namespace utils {

    REGISTER_FOR_SERIALIZATION(ctvty::utils, Vector3D);

    Vector3D::		Vector3D()
      : Vector3D(zero) {}

    Vector3D::		Vector3D(float _x, float _y, float _z)
      : x(_x), y(_y), z(_z) { }

    Vector3D::		Vector3D(const serialization::Archive& __serial) {
      __serial["x"] & x;
      __serial["y"] & y;
      __serial["z"] & z;
    }

    void		Vector3D::Serialize(serialization::Archive& __serial_instance) {
      SERIALIZE_OBJECT_AS(ctvty::utils::Vector3D, __serial_instance);
      __serial["x"] & x;
      __serial["y"] & y;
      __serial["z"] & z;
    }


    const Vector3D	Vector3D::right		( 1.,  0.,  0.);
    const Vector3D	Vector3D::left		(-1.,  0.,  0.);
    const Vector3D	Vector3D::up		( 0.,  1.,  0.);
    const Vector3D	Vector3D::down		( 0., -1.,  0.);
    const Vector3D	Vector3D::forward	( 0.,  0.,  1.);
    const Vector3D	Vector3D::back		( 0.,  0., -1.);
    const Vector3D	Vector3D::zero		( 0.,  0.,  0.);
    const Vector3D	Vector3D::one		( 1.,  1.,  1.);


    Vector3D		Vector3D::GetNormalized() const{
      float magnitude = GetMagnitude();
      return Vector3D(x / magnitude, y / magnitude, z / magnitude);
    }

    float		Vector3D::GetMagnitude() const{
      return std::sqrt(x*x + y*y + z*z);
    }



    Vector3D		Vector3D::Project(const Vector3D& vector) {
      return vector * DotProduct(vector);
    }

    Vector3D		Vector3D::ProjectOnPlane(const Vector3D& normal) {
      return Project(normal) - (*this);
    }

    Vector3D		Vector3D::Reflect(const Vector3D& normal) {
      return (normal * (-2 * DotProduct(normal))) + (*this);
    }



    float		Vector3D::DotProduct(const Vector3D& vector) {
      return
	vector.x * x +
	vector.y * y +
	vector.z * z ;
    }



    Vector3D		Vector3D::operator * (float f) const {
      return Vector3D(x * f, y *f, z *f);
    }

    Vector3D		Vector3D::operator / (float f) const {
      return Vector3D(x / f, y / f, z / f);
    }

    Vector3D&		Vector3D::operator *= (float f) {
      x *= f;
      y *= f;
      z *= f;
      return *this;
    }

    Vector3D&		Vector3D::operator /= (float f) {
      x /= f;
      y /= f;
      z /= f;
      return *this;
    }
    


    Vector3D		Vector3D::operator - () const {
      return Vector3D(-x, -y, -z);
    }

    Vector3D		Vector3D::operator - (const Vector3D& vector) const {
      return Vector3D(x - vector.x,
		      y - vector.y,
		      z - vector.z);
    }

    Vector3D		Vector3D::operator + (const Vector3D& vector) const {
      return Vector3D(x + vector.x,
		      y + vector.y,
		      z + vector.z);
    }

    Vector3D&		Vector3D::operator += (const Vector3D& vector) {
      x += vector.x;
      y += vector.y;
      z += vector.z;
      return *this;
    }

    Vector3D&		Vector3D::operator -= (const Vector3D& vector) {
      x -= vector.x;
      y -= vector.y;
      z -= vector.z;
      return *this;
    }
  };
};
