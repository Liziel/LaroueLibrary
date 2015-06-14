#include "user_defined/component/menu_in_game.hh"
#include "ctvty/application.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, MenuInGame);

namespace user_defined {
  namespace component {

    MenuInGame::MenuInGame(const serialization::Archive&)
      : ctvty::MonoBehaviour<MenuInGame>(nullptr, "MenuInGame") {
      RegisterListener("exit over", &MenuInGame::OverExit);
      RegisterListener("exit OnOver", &MenuInGame::OnOverExit);
      RegisterListener("exit click", &MenuInGame::Exit);
      RegisterListener("play over", &MenuInGame::OverPlay);
      RegisterListener("play OnOver", &MenuInGame::OnOverPlay);
      RegisterListener("play click", &MenuInGame::Play);
    }

    void		MenuInGame::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::MenuInGame, __serial_instance);
      (void)__serial;
    }

    void		MenuInGame::OnDestroy() {
      BroadcastAll("Resume");
    }

    void		MenuInGame::Play(ctvty::component::Hud*)
    {
      Object::Destroy(gameObject);
    }

    void		MenuInGame::OverPlay(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["play overed"]->enable();
      hud->GetCanvas()["play"]->disable();
    }

    void		MenuInGame::OnOverPlay(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["play overed"]->disable();
      hud->GetCanvas()["play"]->enable();
    }

    void		MenuInGame::OverExit(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["exit overed"]->enable();
      hud->GetCanvas()["exit"]->disable();
    }

    void		MenuInGame::OnOverExit(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["exit overed"]->disable();
      hud->GetCanvas()["exit"]->enable();
    }

    void		MenuInGame::Exit(ctvty::component::Hud*)
    {
      ctvty::Application::LoadScene("menu principal");
    }
  };
};
