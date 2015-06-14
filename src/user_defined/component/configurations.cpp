#include "user_defined/component/configurations.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, Configuration);

namespace user_defined {
  namespace component {
    
    Configuration::Configuration(const serialization::Archive& __serial) {
      __serial["size x"]	& _sizeX;
      __serial["size y"]	& _sizeY;
      
      __serial["players"]	& _player;
      __serial["ia"]		& _IA;
    }

    void	Configuration::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::Configuration, __serial_instance);
      __serial["size x"]	& _sizeX;
      __serial["size y"]	& _sizeY;
      
      __serial["players"]	& _player;
      __serial["ia"]		& _IA;
    }
  };
};
