#include "user_defined/component/options_selection.hh"
#include "ctvty/application.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, OptionsSelection);

namespace user_defined
{
    namespace component
    {

    OptionsSelection::OptionsSelection(const serialization::Archive&)
      : ctvty::MonoBehaviour<OptionsSelection>(nullptr, "OptionsSelection")
      {
	  RegisterListener("video over", &OptionsSelection::OverVideo);
	  RegisterListener("video OnOver", &OptionsSelection::OnOverVideo);
	  RegisterListener("video click", &OptionsSelection::Video);
	  RegisterListener("configuration over", &OptionsSelection::OverConfiguration);
	  RegisterListener("configuration OnOver", &OptionsSelection::OnOverConfiguration);
	  RegisterListener("configuration click", &OptionsSelection::Configuration);
	  RegisterListener("shortcuts over", &OptionsSelection::OverShortcuts);
	  RegisterListener("shortcuts OnOver", &OptionsSelection::OnOverShortcuts);
	  RegisterListener("shortcuts click", &OptionsSelection::Shortcuts);
	  RegisterListener("exit over", &OptionsSelection::OverExit);
	  RegisterListener("exit OnOver", &OptionsSelection::OnOverExit);
	  RegisterListener("exit click", &OptionsSelection::Exit);
      }

      void		OptionsSelection::Serialize(serialization::Archive& __serial_instance) const
      {
	SERIALIZE_OBJECT_AS(user_defined::component::OptionsSelection, __serial_instance);
	(void)__serial;
      }

      void		OptionsSelection::OverVideo(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["video overed"]->enable();
	hud->GetCanvas()["video"]->disable();
      }

      void		OptionsSelection::OnOverVideo(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["video overed"]->disable();
	hud->GetCanvas()["video"]->enable();
      }

      void		OptionsSelection::Video(ctvty::component::Hud*)
      {
	ctvty::Application::LoadScene("menu video");
      }

      void		OptionsSelection::OverConfiguration(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["configuration overed"]->enable();
	hud->GetCanvas()["configuration"]->disable();
      }

      void		OptionsSelection::OnOverConfiguration(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["configuration overed"]->disable();
	hud->GetCanvas()["configuration"]->enable();
      }

      void		OptionsSelection::Configuration(ctvty::component::Hud*)
      {
	ctvty::Application::LoadScene("menu configuration");
      }

      void		OptionsSelection::OverShortcuts(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["shortcuts overed"]->enable();
	hud->GetCanvas()["shortcuts"]->disable();
      }

      void		OptionsSelection::OnOverShortcuts(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["shortcuts overed"]->disable();
	hud->GetCanvas()["shortcuts"]->enable();
      }

      void	        OptionsSelection::Shortcuts(ctvty::component::Hud*)
      {
	ctvty::Application::LoadScene("menu options");
      }

      void		OptionsSelection::OverExit(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["exit overed"]->enable();
	hud->GetCanvas()["exit"]->disable();
      }

      void		OptionsSelection::OnOverExit(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["exit overed"]->disable();
	hud->GetCanvas()["exit"]->enable();
      }

      void		OptionsSelection::Exit(ctvty::component::Hud*)
      {
	ctvty::Application::LoadScene("menu principal");
      }
    };
};
