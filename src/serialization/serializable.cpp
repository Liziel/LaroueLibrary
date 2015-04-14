#include "serialization/serializable.hh"
#include "serialization/archive.hh"
#include "serialization/serial.hh"


namespace serialization {

  std::map< std::string, std::function<Serializable*(const Archive&)> >&
				Serializable::getAllocationMap() {
    static std::map< std::string, std::function<Serializable*(const Archive&)> > _intern;

    return (_intern);
  }

  std::map< std::string, std::string >&
				Serializable::getTypeIdMap() {
    static std::map< std::string, std::string > _intern;

    return (_intern);
  }

  Serializable*			Serializable::Instantiate(const Archive& archive) {
    return getAllocationMap() [archive["type"].as<std::string>()] (archive["object"].as<const Archive>());
  }

  Serializable*			Serializable::Instantiate(const Serial& serial) {
    const Archive&		archive = serial.as<Archive>();
    return getAllocationMap() [archive["type"].as<std::string>()] (archive["object"].as<const Archive>());
  }
};
