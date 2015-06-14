#include "user_defined/component/map_generator.hh"
#include "ctvty/application.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, MapGenerator);

namespace user_defined {
  namespace component {
    MapGenerator::MapGenerator(const serialization::Archive& __serial)
      : MonoBehaviour(nullptr, "MapGenerator") {
      __serial["ground"]	& ground;
      __serial["destructible"]	& destructible;
      __serial["indestructible"]& indestructible;

      __serial["player 1"]	& player1;
      __serial["player 2"]	& player2;

      __serial["bombes"]	& bombes;
    }

    void		MapGenerator::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::MapGenerator, __serial_instance);
      __serial["ground"]	& ground;
      __serial["destructible"]	& destructible;
      __serial["indestructible"]& indestructible;

      __serial["player 1"]	& player1;
      __serial["player 2"]	& player2;

      __serial["bombes"]	& bombes;
    }

    void		MapGenerator::Awake() {
      std::shared_ptr<Configuration> configuration =
	ctvty::Application::Assets().GetAsset("saves/configurations.json").LoadAs<Configuration>();
      size_x = configuration->_sizeX;
      size_y = configuration->_sizeY;
      for (auto& bombe : bombes)
	bombe->delayedInstantiation();
      Generate(configuration->_player, configuration->_IA);
    }

    void		MapGenerator::Generate(int players, int ias) {
      for (std::size_t x = 0; x < size_x; ++x)
	for (std::size_t y = 0; y < size_y; ++y) {
	  Object::Instantiate(ground.get(),
			      ctvty::utils::Vector3D((int)((size_x/ 2) - x), 0, (int)(size_y/ 2 - y)),
			      ctvty::utils::Quaternion::identity);
	  if ((x == 1 && y == 1) || (x == 1 && y == 2) || (x == 2 && y == 1));
	  else if ((x == 1 && y == size_y - 2) || (x == 1 && y == size_y - 3) || (x == 2 && y == size_y - 2));
	  else if ((y == 1 && x == size_x - 2) || (y == 1 && x == size_x - 3) || (y == 2 && x == size_x - 2));
	  else if ((y == size_y - 2 && x == size_x - 2)
		   || (y == size_y - 2 && x == size_x - 3) || (y == size_y - 3 && x == size_x - 2));
	  else if (x == size_x - 1 || x == 0)
	    Object::Instantiate(indestructible.get(),
				ctvty::utils::Vector3D((int)((size_x/ 2) - x), 1, (int)((size_y/ 2) - y)),
				ctvty::utils::Quaternion::identity);
	  else if (y == size_y - 1 || y == 0)
	    Object::Instantiate(indestructible.get(),
				ctvty::utils::Vector3D((int)((size_x/ 2) - x), 1, (int)((size_y/ 2) - y)),
				ctvty::utils::Quaternion::identity);
	  else if (y == size_y - 2 || y == 1)
	    Object::Instantiate(destructible.get(),
				ctvty::utils::Vector3D((int)((size_x/ 2) - x), 1, (int)((size_y/ 2) - y)),
				ctvty::utils::Quaternion::identity);
	  else if (x == size_x - 2 || x == 1)
	    Object::Instantiate(destructible.get(),
				ctvty::utils::Vector3D((int)((size_x/ 2) - x), 1, (int)((size_y/ 2) - y)),
				ctvty::utils::Quaternion::identity);
	  else if (x % 2 == 1)
	    Object::Instantiate(destructible.get(),
				ctvty::utils::Vector3D((int)((size_x/ 2) - x), 1, (int)((size_y/ 2) - y)),
				ctvty::utils::Quaternion::identity);
	  else if (y % 2 == 0)
	    Object::Instantiate(indestructible.get(),
				ctvty::utils::Vector3D((int)((size_x/ 2) - x), 1, (int)((size_y/ 2) - y)),
				ctvty::utils::Quaternion::identity);
	  else
	    Object::Instantiate(destructible.get(),
				ctvty::utils::Vector3D((int)((size_x/ 2) - x), 1, (int)((size_y/ 2) - y)),
				ctvty::utils::Quaternion::identity);
	}
      Object::Instantiate(player1.get(),
			  ctvty::utils::Vector3D(size_x/2 - 1, 3, size_x/2 - 1),
			  ctvty::utils::Quaternion::identity);
      if (players == 2)
	Object::Instantiate(player2.get(),
			    ctvty::utils::Vector3D(size_x/2 - 1, 3, size_x/2 - 1),
			    ctvty::utils::Quaternion::identity);
    }
  };
};
