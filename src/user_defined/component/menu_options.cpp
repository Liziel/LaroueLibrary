#include "user_defined/component/menu_options.hh"
#include "ctvty/application.hh"
#include "ctvty/event.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, MenuOptions);

namespace user_defined {
  namespace component {

    MenuOptions::MenuOptions(const serialization::Archive&)
      : ctvty::MonoBehaviour<MenuOptions>(nullptr, "MenuOptions") {
      RegisterListener("exit over", &MenuOptions::OverExit);
      RegisterListener("exit OnOver", &MenuOptions::OnOverExit);
      RegisterListener("exit click", &MenuOptions::Exit);
      RegisterListener("OnSetterClick", &MenuOptions::OnSetterClick);
      setting = false;
    }

    void		MenuOptions::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::MenuOptions, __serial_instance);
      (void)__serial;
    }

    void		MenuOptions::OnSetterClick(ctvty::component::Hud* hud) 
    {
      setting = true;
      setted = hud;
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
      ctvty::Application::LoadScene("menu principal");
    }

    void		MenuOptions::OnGui() 
    {
      auto e = ctvty::Event::current();

      if (!setting)
	return ;
      if (e->type() != ctvty::Event::Type::keydown)
	return ;
      std::cout << e->keycode() << std::endl;
      if (e->keycode() >= 'a' && e->keycode() <= 'z') {
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(e->keycode() - 'a' + 'A') + ".json"));
	texture->delayedInstantiation();
	setted->SetTexture(texture);
      } else if (e->keycode() >= 1073741912 && e->keycode() <= 1073741921) {
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(e->keycode() - 1073741912 + 48)
					        + ".json"));
	texture->delayedInstantiation();
	setted->SetTexture(texture);	  
      } else {
	switch(e->keycode()) {
	case 1073741906:
	  texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						  "Arrow_up.json"));
	  break;
	case 1073741905:
	  texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						  "Arrow_down.json"));
	  break;
	case 1073741904:
	  texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						  "Arrow_left.json"));
	  break;
	case 1073741903:
	  texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						  "Arrow_right.json"));
	  break;
	case 32:
	  texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						  "Space.json"));
	  break;
	case 1073741922:
	  texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						  "0.json"));
	  break;
	default:
	  setting = false;
	  setted = nullptr;
	  return ;
	}
	texture->delayedInstantiation();
	setted->SetTexture(texture);
      }
      setting = false;
      setted = nullptr;
    }
    
  };
};
