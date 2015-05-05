#ifndef Quaternion_hh__
# define Quaternion_hh__

# include "serialization/serializable.hh"

# include "ctvty/utils/vector3d.hh"

namespace ctvty {
  namespace utils {

    class Quaternion : public serialization::Serializable {
    private:
      float	x,y,z,w;

    public:
      static const Quaternion	identity;

    public:
      Quaternion(const Quaternion&) = default;

    public:
      Quaternion(float, float, float, float);
      Quaternion(const Vector3D&, float);
      Quaternion(const serialization::Archive&);

    public:
      inline//for implicit cast convertion
      Quaternion(const Vector3D& v) : Quaternion(v, 0) {}

    public:
      ~Quaternion();

    public:
      void		Serialize(serialization::Archive& __serial) const override;


    public:
      const Vector3D	Complex() const;
      float		Real() const;

    public:
      float		Magnitude() const;

    public:
      static
      const Quaternion	Euler(const Vector3D&);
      const Vector3D	Euler() const;
      const Quaternion	Conjugate() const;
      const Quaternion	Inverse() const;

    public:
      const Vector3D	RotatedVector(const Vector3D&) const;
      const Vector3D	RotatedAround(const Vector3D& center, const Vector3D&) const;

    public:
      const Quaternion	operator * (const Quaternion& rhs) const;
      const Quaternion	operator * (float) const;
    };

    inline
    const Quaternion	operator * (const Vector3D&v, const Quaternion& q) {
      return {q * static_cast<const Quaternion>(v)};
    }

    inline
    const Quaternion	operator * (float f, const Quaternion& q) {
      return (q * f);
    }
  };
};

#endif
