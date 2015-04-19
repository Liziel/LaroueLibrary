#ifndef Vector3d_hh__
# define Vector3d_hh__

# include "serialization/serializable.hh"

namespace ctvty {
  namespace utils {
    
    class Vector3D : public serialization::Serializable{
    public:
      float	x,y,z;
      float	scale;

    public:
      Vector3D(float, float, float, float = 1.);
      Vector3D(const serialization::Archive&);

    public:
      void	Serialize(serialization::Archive& __serial) override;
    };

  };
};

#endif
