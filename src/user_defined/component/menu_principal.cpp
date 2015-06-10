#include "user_defined/component/menu_principal.hh"
#include "ctvty/application.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, MenuPrincipal);

namespace user_defined {
  namespace component {

    MenuPrincipal::MenuPrincipal(const serialization::Archive&)
      : ctvty::MonoBehaviour<MenuPrincipal>(nullptr, "MenuPrincipal") {
      RegisterListener("exit over", &MenuPrincipal::OverExit);
      RegisterListener("exit OnOver", &MenuPrincipal::OnOverExit);
      RegisterListener("exit click", &MenuPrincipal::Exit);
      RegisterListener("play over", &MenuPrincipal::OverPlay);
      RegisterListener("play OnOver", &MenuPrincipal::OnOverPlay);
      RegisterListener("play click", &MenuPrincipal::Play); // Lancement du jeu
      RegisterListener("options over", &MenuPrincipal::OverOptions);
      RegisterListener("options OnOver", &MenuPrincipal::OnOverOptions);
      RegisterListener("options click", &MenuPrincipal::Options); // Lancement menu des options
      RegisterListener("credit over", &MenuPrincipal::OverCredit);
      RegisterListener("credit OnOver", &MenuPrincipal::OnOverCredit);
      //RegisterListener("credit click", &); // Lancement des credits
    }

    void		MenuPrincipal::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::MenuPrincipal, __serial_instance);
      (void)__serial;
    }

    void		MenuPrincipal::Play(ctvty::component::Hud*) {
      ctvty::Application::LoadScene("game template");
    }

    void		MenuPrincipal::OverPlay(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["play overed"]->enable();
      hud->GetCanvas()["play"]->disable();
    }

    void		MenuPrincipal::OnOverPlay(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["play overed"]->disable();
      hud->GetCanvas()["play"]->enable();
    }

    void		MenuPrincipal::Options(ctvty::component::Hud*)
    {
      ctvty::Application::LoadScene("menu options");
    }

    void		MenuPrincipal::OverOptions(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["options overed"]->enable();
      hud->GetCanvas()["options"]->disable();
    }

    void		MenuPrincipal::OnOverOptions(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["options overed"]->disable();
      hud->GetCanvas()["options"]->enable();
    }

    void		MenuPrincipal::OverCredit(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["credit overed"]->enable();
      hud->GetCanvas()["credit"]->disable();
    }

    void		MenuPrincipal::OnOverCredit(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["credit overed"]->disable();
      hud->GetCanvas()["credit"]->enable();
    }

    void		MenuPrincipal::OverExit(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["exit overed"]->enable();
      hud->GetCanvas()["exit"]->disable();
    }

    void		MenuPrincipal::OnOverExit(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["exit overed"]->disable();
      hud->GetCanvas()["exit"]->enable();
    }

    void		MenuPrincipal::Exit(ctvty::component::Hud*)
    {
      ctvty::Application::Quit();
    }
  };
};
