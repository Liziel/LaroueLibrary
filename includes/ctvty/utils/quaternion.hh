#ifndef Quaternion_hh__
# define Quaternion_hh__

# include "serialization/serializable.hh"

# include "ctvty/utils/vector3d.hh"

namespace ctvty {
  namespace utils {

    class Quaternion : public serialization::Serializable {
    private:
      Vector3D*	rotation;
      float	scale;

    public:
      Quaternion(float, float, float, float);
      Quaternion(const Vector3D&, float);
      Quaternion(const Quaternion&);
      Quaternion(const serialization::Archive&);

    public:
      ~Quaternion();

    public:
      void	Serialize(serialization::Archive& __serial) override;
    };

  };
};

#endif
