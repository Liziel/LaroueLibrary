#include "user_defined/helper/tga_dict.hh"

REGISTER_FOR_SERIALIZATION(user_defined::helper, TgaDict);

namespace user_defined {
  namespace helper {
    
    TgaDict::		TgaDict(const serialization::Archive& __serial) {
      __serial["dictionary"]	& dict;
    }

    void		TgaDict::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::helper::TgaDict, __serial_instance);
      __serial["dictionary"]	& dict;
    }

    std::shared_ptr< ctvty::asset::Police >		TgaDict::operator[](const char c) {
      return dict[c];
    }
  };
};
