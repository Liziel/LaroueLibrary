#include "user_defined/component/menu_options.hh"
#include "ctvty/application.hh"
#include "ctvty/event.hh"
#include "ctvty/input.hh"

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

    void	MenuOptions::Awake()
    {
      if (GetComponent<ctvty::component::Canvas>() == nullptr)
	return ;
      ctvty::component::Canvas&	canvas = *GetComponent<ctvty::component::Canvas>();
      std::list<std::string>	inputs = 
	{
	  "up player 1", "up player 2", "down player 1", "down player 2", "left player 1",
	  "left player 2", "right player 1", "right player 2", "put bomb player 1",
	  "put bomb player 2", "throw bomb player 1", "throw bomb player 2"
	};
      std::for_each(inputs.begin(), inputs.end(), [this, &canvas] (const std::string& name) {
	  if (ctvty::Input::ConfigurationExist(name) && ctvty::Input::GetInputInfo(name).associated == ctvty::Input::configuration::type::key)
	    SetHud(canvas[name].get(), ctvty::Input::GetInputInfo(name).key);
	});
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
      ctvty::Application::LoadScene("options selection");
    }

    void		MenuOptions::SetHud(ctvty::component::Hud* hud, int keycode)
    {
      if (keycode >= 'a' && keycode <= 'z') {
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(keycode - 'a' + 'A') + ".json"));
	texture->delayedInstantiation();
	std::cout << "texture " << hud << std::endl;
	hud->SetTexture(texture);
      } else if (keycode >= 1073741912 && keycode <= 1073741921) {
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(keycode - 1073741864)
					        + ".json"));
	texture->delayedInstantiation();
	hud->SetTexture(texture);	  
      } else {
	switch(keycode) {
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
	  hud = nullptr;
	  return ;
	}
	texture->delayedInstantiation();
	hud->SetTexture(texture);
      }
    }    

    void		MenuOptions::OnGui() 
    {
      auto e = ctvty::Event::current();

      if (!setting)
	return ;
      if (e->type() != ctvty::Event::Type::keydown)
	return ;
      std::cout << e->keycode() << std::endl;
      SetHud(setted, e->keycode());
      ctvty::Input::AssignInput(setted->GetName(), e.get());
      setting = false;
      setted = nullptr;
    }
    
  };
};
