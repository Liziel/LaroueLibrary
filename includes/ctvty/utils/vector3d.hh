#ifndef Vector3d_hh__
# define Vector3d_hh__

# include "serialization/serializable.hh"

namespace ctvty {
  namespace utils {
    
    struct Vector3D : public serialization::Serializable{
    public:
      float	x,y,z;
      float	scale;

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
      Vector3D(float, float, float, float = 1.);
      Vector3D(const serialization::Archive&);

    public:
      void	Serialize(serialization::Archive& __serial) override;

    public:
      Vector3D	GetNormalized();
      float	GetMagnitude();

    public:
      Vector3D	Reflect(const Vector3D& normal);
      Vector3D	Project(const Vector3D& vector);
      Vector3D	ProjectOnPlane(const Vector3D& normal);

    public:
      Vector3D	operator + (float) const;
      Vector3D	operator + (const Vector3D&) const;

    public:
      Vector3D	operator - () const;
      Vector3D	operator - (float) const;
      Vector3D	operator - (const Vector3D&) const;

    public:
      Vector3D	operator * (float) const;
      Vector3D	operator * (const Vector3D&) const;
      Vector3D	operator / (float) const;
      Vector3D	operator / (const Vector3D&) const;

    public:
      Vector3D&	operator += (float);
      Vector3D&	operator += (const Vector3D&);
      
    public:
      Vector3D&	operator *= (float);
      Vector3D&	operator *= (const Vector3D&);
    };

  };
};

#endif
