#include "user_defined/component/map_generator.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, MapGenrator);

namespace user_defined {
  namespace component {
    MapGenrator::MapGenrator(const serialization::Archive& __serial) {
      __serial["ground"]	& ground;
      __serial["destructible"]	& destructible;
      __serial["indestructible"]& indestrucitble;

      __serial["size x"]	& size_x;
      __serial["size y"]	& size_y;
    }

    void		MapGenerator::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::MapGenerator, __serial_instance);
      __serial["ground"]	& ground;
      __serial["destructible"]	& destructible;
      __serial["indestructible"]& indestrucitble;

      __serial["size x"]	& size_x;
      __serial["size y"]	& size_y;

      if (__serial.exist("directly generated"))
	__serial["directly generated"]	& directly_generated;
    }

    void		MapGenerator::Awake() {
      if (directly_generated)
	OnLoadScene("");
    }

    void		MapGenerator::OnLoadScene(const std::string&) {
      for (std::size_t x = 0; x < size_x; ++x)
	for (std::size_t y = 0; y < size_y; ++y)
	  Object::Instantiate(ground,
			      ctvty::utils::Vector3D(size_x/ 2 - x, size_y/ 2 - y, 0),
			      ctvty::utils::Quaternion::identity);
    }
  };
};
