#include "user_defined/component/menu_video.hh"
#include "ctvty/application.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, MenuVideo);

namespace user_defined
{
    namespace component
    {

    MenuVideo::MenuVideo(const serialization::Archive&)
      : ctvty::MonoBehaviour<MenuVideo>(nullptr, "MenuVideo")
      {
	this->_resolution = 0;
	this->_fullscreen = true;
	RegisterListener("1920x1080 click", &MenuVideo::Resolution1);
	RegisterListener("1366x768 click", &MenuVideo::Resolution2);
	RegisterListener("800x600 click", &MenuVideo::Resolution3);
	RegisterListener("on click", &MenuVideo::OnClick);
	RegisterListener("off click", &MenuVideo::OffClick);
	RegisterListener("apply over", &MenuVideo::OverApply);
	RegisterListener("apply OnOver", &MenuVideo::OnOverApply);
	RegisterListener("apply click", &MenuVideo::Apply);
	RegisterListener("exit over", &MenuVideo::OverExit);
	RegisterListener("exit OnOver", &MenuVideo::OnOverExit);
	RegisterListener("exit click", &MenuVideo::Exit);
      }

      void		MenuVideo::Serialize(serialization::Archive& __serial_instance) const
      {
	SERIALIZE_OBJECT_AS(user_defined::component::MenuVideo, __serial_instance);
	(void)__serial;
      }

      void		MenuVideo::Resolution1(ctvty::component::Hud* hud)
      {
	this->_resolution = 0;
	hud->GetCanvas()["1920x1080 overed"]->enable();
	hud->GetCanvas()["1920x1080"]->disable();
	hud->GetCanvas()["1366x768 overed"]->disable();
	hud->GetCanvas()["1366x768"]->enable();
	hud->GetCanvas()["800x600 overed"]->disable();
	hud->GetCanvas()["800x600"]->enable();
      }

      void		MenuVideo::Resolution2(ctvty::component::Hud* hud)
      {
	this->_resolution = 1;
	hud->GetCanvas()["1366x768 overed"]->enable();
	hud->GetCanvas()["1366x768"]->disable();
	hud->GetCanvas()["1920x1080 overed"]->disable();
	hud->GetCanvas()["1920x1080"]->enable();
	hud->GetCanvas()["800x600 overed"]->disable();
	hud->GetCanvas()["800x600"]->enable();
      }

      void		MenuVideo::Resolution3(ctvty::component::Hud* hud)
      {
	this->_resolution = 2;
	hud->GetCanvas()["800x600 overed"]->enable();
	hud->GetCanvas()["800x600"]->disable();
	hud->GetCanvas()["1366x768 overed"]->disable();
	hud->GetCanvas()["1366x768"]->enable();
	hud->GetCanvas()["1920x1080 overed"]->disable();
	hud->GetCanvas()["1920x1080"]->enable();
      }

      void		MenuVideo::OnClick(ctvty::component::Hud* hud)
      {
	this->_fullscreen = true;
	hud->GetCanvas()["off overed"]->disable();
	hud->GetCanvas()["off"]->enable();
	hud->GetCanvas()["on overed"]->enable();
	hud->GetCanvas()["on"]->disable();
      }

      void		MenuVideo::OffClick(ctvty::component::Hud* hud)
      {
	this->_fullscreen = false;
	hud->GetCanvas()["off overed"]->enable();
	hud->GetCanvas()["off"]->disable();
	hud->GetCanvas()["on overed"]->disable();
	hud->GetCanvas()["on"]->enable();
      }

      void		MenuVideo::OverApply(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["apply overed"]->enable();
	hud->GetCanvas()["apply"]->disable();
      }

      void		MenuVideo::OnOverApply(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["apply overed"]->disable();
	hud->GetCanvas()["apply"]->enable();
      }

      void		MenuVideo::Apply(ctvty::component::Hud*)
      {
	// this->_resolution (0->2) and this->_fullscreen (true, false).
      }

      void		MenuVideo::OverExit(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["exit overed"]->enable();
	hud->GetCanvas()["exit"]->disable();
      }

      void		MenuVideo::OnOverExit(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["exit overed"]->disable();
	hud->GetCanvas()["exit"]->enable();
      }

      void		MenuVideo::Exit(ctvty::component::Hud*)
      {
	ctvty::Application::LoadScene("options selection");
      }

    };
};
