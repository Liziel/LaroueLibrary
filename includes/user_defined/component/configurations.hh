#ifndef Configuration_hh__
# define Configuration_hh__

# include "serialization/serializable.hh"

namespace user_defined {
  namespace component {
    
    struct Configuration : public serialization::Serializable {
      Configuration(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

      int       _sizeX;
      int       _sizeY;
      int       _player;
      int       _IA;
    };
  };
};

#endif
