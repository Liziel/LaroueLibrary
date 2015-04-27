#ifndef Material_hh__
# define Material_hh__

# include "serialization/serializable.hh"

namespace ctvty {
  namespace physics {

    class Material : public serialization::Serializable {
    public:
      float	bounciness;
      float	staticFriction;
      float	dynamicFriction;

    public:
      void	Serialize(serialization::Archive&) const;
    };
  };
};

#endif
