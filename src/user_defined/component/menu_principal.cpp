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
      RegisterListener("ladder over", &MenuPrincipal::OverLadder);
      RegisterListener("ladder OnOver", &MenuPrincipal::OnOverLadder);
      RegisterListener("load game over", &MenuPrincipal::OverLoadGame);
      RegisterListener("load game OnOver", &MenuPrincipal::OnOverLoadGame);
      //RegisterListener("load game click", &MenuPrincipal::LoadGame); // Lancement du menu de load des games
      RegisterListener("ladder click", &MenuPrincipal::Ladder);
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
      ctvty::Application::LoadScene("options selection");
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

    void		MenuPrincipal::OverLoadGame(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["load game overed"]->enable();
      hud->GetCanvas()["load game"]->disable();
    }

    void		MenuPrincipal::OnOverLoadGame(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["load game overed"]->disable();
      hud->GetCanvas()["load game"]->enable();
    }

    void		MenuPrincipal::OverLadder(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["ladder overed"]->enable();
      hud->GetCanvas()["ladder"]->disable();
    }

    void		MenuPrincipal::OnOverLadder(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["ladder overed"]->disable();
      hud->GetCanvas()["ladder"]->enable();
    }

    void		MenuPrincipal::Ladder(ctvty::component::Hud*)
    {
      ctvty::Application::LoadScene("menu ladder");
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
