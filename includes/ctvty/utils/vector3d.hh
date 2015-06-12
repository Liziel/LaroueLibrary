#ifndef Vector3d_hh__
# define Vector3d_hh__

# include "serialization/serializable.hh"

namespace ctvty {
  namespace utils {
    
    struct Vector3D : public serialization::Serializable{
    public:
      float	x,y,z;

    public:
      static const Vector3D	up;
      static const Vector3D	down;
      static const Vector3D	right;
      static const Vector3D	left;
      static const Vector3D	forward;
      static const Vector3D	back;
      static const Vector3D	zero;
      static const Vector3D	one;

    public:
      Vector3D();
      Vector3D(float, float, float);
      Vector3D(const serialization::Archive&);

    public:
      void	Serialize(serialization::Archive& __serial) const override;

    public:
      const Vector3D	GetNormalized() const;
      float		GetMagnitude() const;

    public:
      const Vector3D	Reflect(const Vector3D& normal) const;
      const Vector3D	Project(const Vector3D& vector) const;
      const Vector3D	ProjectOnPlane(const Vector3D& normal) const;

    public:
      float		DotProduct(const Vector3D& vector) const;
      const Vector3D	CrossProduct(const Vector3D& vector) const;
      float		ScalarProjection(const Vector3D& vector) const;

    public:
      const Vector3D	operator - () const;
      const Vector3D	operator + (const Vector3D&) const;
      const Vector3D	operator - (const Vector3D&) const;
      const Vector3D	operator * (const Vector3D&) const;
      Vector3D&		operator += (const Vector3D&);
      Vector3D&		operator -= (const Vector3D&);
      Vector3D&		operator *= (const Vector3D&);

    public:
      const Vector3D	operator * (float) const;
      const Vector3D	operator / (float) const;
      Vector3D&		operator *= (float);
      Vector3D&		operator /= (float);

    public:
      bool		operator == (const Vector3D&) const;
    };


    inline
    const Vector3D	operator * (float f, const Vector3D& v) { return v * f; }

    inline
    const Vector3D	operator / (float f, const Vector3D& v) { return v / f; }
  };
};

#endif
