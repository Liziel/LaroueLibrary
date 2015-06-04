#include "user_defined/component/menu_principal.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, MenuPrincipal);

namespace user_defined {
  namespace component {

    MenuPrincipal::	MenuPrincipal(const serialization::Archive&)
      : MonoBehaviour("MenuPrincipal", nullptr) {
      RegisterListener("exit over", &MenuPrincipal::OverExit);
    }

    void		MenuPrincipal(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::MenuPrincipal, __serial_instance);
      (void)__serial;
    }

    void		OverExit(ctvty::component::Hud* hud) {
      std::cout << "je suis appelé lors du passage de la souris au dessus de exit" << std::endl;
    }
    
  };
};
