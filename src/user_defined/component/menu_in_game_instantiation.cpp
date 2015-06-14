#include "user_defined/component/menu_in_game_instantiation.hh"
#include "ctvty/input.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, MenuInGameInstantiation);

namespace user_defined {
  namespace component {
    MenuInGameInstantiation::MenuInGameInstantiation(const serialization::Archive& __serial)
      : ctvty::MonoBehaviour<MenuInGameInstantiation>(nullptr, "In Game Insantiation"),
	loaded(false) {
      __serial["Menu InGame"] & MenuInGame;
      RegisterListener("Resume", &MenuInGameInstantiation::Resume);
    }

    void	MenuInGameInstantiation::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::MenuInGameInstantiation, __serial_instance);
      __serial["Menu InGame"] & MenuInGame;
    }

    void	MenuInGameInstantiation::Update() {
      if (!loaded && ctvty::Input::GetKeyState("Menu In Game")) {
	loaded = true;
	instantied = (ctvty::GameObject*)Object::Instantiate(MenuInGame.get());
      } else if (ctvty::Input::GetKeyState("Menu In Game")) {
	Object::Destroy(instantied);
      }
    }

    void	MenuInGameInstantiation::Resume() {
      std::cout << "resume" << std::endl;
      loaded = false;
      instantied = nullptr;
    }

  };
};
