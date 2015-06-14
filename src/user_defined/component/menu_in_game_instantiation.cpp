#include "user_defined/component/menu_in_game_instantiation.hh"
#include "ctvty/input.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, MenuInGameInstantiation);

namespace user_defined {
  namespace component {
    MenuInGameInstantiation::MenuInGameInstantiation(const serialization::Archive& __serial)
      : ctvty::MonoBehaviour<MenuInGameInstantiation>(nullptr, "In Game Insantiation"), loaded(false) {
      __serial["Menu InGame"] & MenuInGame;
      RegisterListener("Resume", &MenuInGameInstantiation::Resume);
    }

    void	MenuInGameInstantiation::Serialize(serialization::Archive& __serial) const {
      SERIALIZE_OBJECT_AS(...);
      __serial["Menu InGame"] & MenuInGame;
    }

    void	MenuInGameInstantiation::Update() {
      if (!loaded && ctvty::Input::GetKeyState("Menu In Game")) {
	loaded = true;
	instantied = Object::Instantiate(MenuInGame.get());
      } else if (ctvty::Input::GetKeyState("Menu In Game")) {
	Object::Destroy(instantied);
      }
    }

    void	MenuInGameInstantiation::Resume() {
      loaded = false;
      instantied = nullptr;
    }

  };
};
