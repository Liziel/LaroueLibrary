#include "user_defined/component/menu_options.hh"
#include "ctvty/application.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, MenuOptions);

namespace user_defined {
  namespace component {

    MenuOptions::MenuOptions(const serialization::Archive&)
      : ctvty::MonoBehaviour<MenuOptions>(nullptr, "MenuOptions") {
      RegisterListener("exit over", &MenuOptions::OverExit);
      RegisterListener("exit OnOver", &MenuOptions::OnOverExit);
      RegisterListener("exit click", &MenuOptions::Exit);
    }

    void		MenuOptions::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::MenuOptions, __serial_instance);
      (void)__serial;
    }

    void		MenuOptions::OverExit(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["exit overed"]->enable();
      hud->GetCanvas()["exit"]->disable();
    }

    void		MenuOptions::OnOverExit(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["exit overed"]->disable();
      hud->GetCanvas()["exit"]->enable();
    }

    void		MenuOptions::Exit(ctvty::component::Hud*)
    {
      ctvty::Application::Quit();
    }
  };
};
