#include "user_defined/component/menu_configuration.hh"
#include "ctvty/application.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, MenuConfiguration);

namespace user_defined
{
    namespace component
    {

      MenuConfiguration::MenuConfiguration(const serialization::Archive& __serial)
	: ctvty::MonoBehaviour<MenuConfiguration>(nullptr, "MenuConfiguration")
      {
  	RegisterListener("apply over", &MenuConfiguration::OverApply);
	RegisterListener("apply OnOver", &MenuConfiguration::OnOverApply);
	RegisterListener("apply click", &MenuConfiguration::Apply);
	RegisterListener("1 player click", &MenuConfiguration::PlayerClick1);
	RegisterListener("2 player click", &MenuConfiguration::PlayerClick2);
	RegisterListener("1 ia click", &MenuConfiguration::IAClick1);
	RegisterListener("2 ia click", &MenuConfiguration::IAClick2);
	RegisterListener("up size x click", &MenuConfiguration::UpSizeX);
	RegisterListener("down size x click", &MenuConfiguration::DownSizeX);
	RegisterListener("up size y click", &MenuConfiguration::UpSizeY);
	RegisterListener("down size y click", &MenuConfiguration::DownSizeY);
	RegisterListener("exit over", &MenuConfiguration::OverExit);
	RegisterListener("exit OnOver", &MenuConfiguration::OnOverExit);
	RegisterListener("exit click", &MenuConfiguration::Exit);
      }

      void		MenuConfiguration::Serialize(serialization::Archive& __serial_instance) const
      {
	SERIALIZE_OBJECT_AS(user_defined::component::MenuConfiguration, __serial_instance);
	(void)__serial;
      }

      void		MenuConfiguration::Awake() {
	configuration =
	  ctvty::Application::Assets().GetAsset("saves/configurations.json").LoadAs<Configuration>();
	ctvty::component::Canvas& canvas = *GetComponent<ctvty::component::Canvas>();
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(configuration->_sizeX % 10 + 48) + ".json"));
	texture->delayedInstantiation();
	canvas["size x u"]->SetTexture(texture);
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(configuration->_sizeX / 10 + 48) + ".json"));
	texture->delayedInstantiation();
	canvas["size x d"]->SetTexture(texture);
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(configuration->_sizeY % 10 + 48) + ".json"));
	texture->delayedInstantiation();
	canvas["size y u"]->SetTexture(texture);
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(configuration->_sizeY / 10 + 48) + ".json"));
	texture->delayedInstantiation();
	canvas["size y d"]->SetTexture(texture);
      }

      void		MenuConfiguration::DownSizeX(ctvty::component::Hud* hud)
      {
	if (configuration->_sizeX != 10)
	  configuration->_sizeX--;
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(configuration->_sizeX % 10 + 48) + ".json"));
	texture->delayedInstantiation();
	hud->GetCanvas()["size x u"]->SetTexture(texture);
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(configuration->_sizeX / 10 + 48) + ".json"));
	texture->delayedInstantiation();
	hud->GetCanvas()["size x d"]->SetTexture(texture);
      }

      void		MenuConfiguration::UpSizeX(ctvty::component::Hud* hud)
      {
	if (configuration->_sizeX != 99)
	  configuration->_sizeX++;
	std::cout << "size x = " << configuration->_sizeX << std::endl;
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(configuration->_sizeX % 10 + 48) + ".json"));
	texture->delayedInstantiation();
	hud->GetCanvas()["size x u"]->SetTexture(texture);
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(configuration->_sizeX / 10 + 48) + ".json"));
	texture->delayedInstantiation();
	hud->GetCanvas()["size x d"]->SetTexture(texture);
      }

      void		MenuConfiguration::DownSizeY(ctvty::component::Hud* hud)
      {
	if (configuration->_sizeY != 10)
	  configuration->_sizeY--;
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(configuration->_sizeY % 10 + 48) + ".json"));
	texture->delayedInstantiation();
	hud->GetCanvas()["size y u"]->SetTexture(texture);
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(configuration->_sizeY / 10 + 48) + ".json"));
	texture->delayedInstantiation();
	hud->GetCanvas()["size y d"]->SetTexture(texture);
      }

      void		MenuConfiguration::UpSizeY(ctvty::component::Hud* hud)
      {
	if (configuration->_sizeY != 99)
	  configuration->_sizeY++;
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(configuration->_sizeY % 10 + 48) + ".json"));
	texture->delayedInstantiation();
	hud->GetCanvas()["size y u"]->SetTexture(texture);
	texture.reset(new ctvty::asset::Texture(std::string("menu/textures/") +
						static_cast<char>(configuration->_sizeY / 10 + 48) + ".json"));
	texture->delayedInstantiation();
	hud->GetCanvas()["size y d"]->SetTexture(texture);
      }

      void		MenuConfiguration::PlayerClick1(ctvty::component::Hud* hud)
      {
	configuration->_player = 1;
	hud->GetCanvas()["2 player overed"]->disable();
	hud->GetCanvas()["2 player"]->enable();
	hud->GetCanvas()["1 player overed"]->enable();
	hud->GetCanvas()["1 player"]->disable();
      }

      void		MenuConfiguration::PlayerClick2(ctvty::component::Hud* hud)
      {
	configuration->_player = 2;
	hud->GetCanvas()["1 player overed"]->disable();
	hud->GetCanvas()["1 player"]->enable();
	hud->GetCanvas()["2 player overed"]->enable();
	hud->GetCanvas()["2 player"]->disable();
      }

      void		MenuConfiguration::IAClick1(ctvty::component::Hud* hud)
      {
	configuration->_IA = 1;
	hud->GetCanvas()["2 ia overed"]->disable();
	hud->GetCanvas()["2 ia"]->enable();
	hud->GetCanvas()["1 ia overed"]->enable();
	hud->GetCanvas()["1 ia"]->disable();
      }

      void		MenuConfiguration::IAClick2(ctvty::component::Hud* hud)
      {
	configuration->_IA = 2;
	hud->GetCanvas()["1 ia overed"]->disable();
	hud->GetCanvas()["1 ia"]->enable();
	hud->GetCanvas()["2 ia overed"]->enable();
	hud->GetCanvas()["2 ia"]->disable();
      }

      void		MenuConfiguration::OverApply(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["apply overed"]->enable();
	hud->GetCanvas()["apply"]->disable();
      }

      void		MenuConfiguration::OnOverApply(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["apply overed"]->disable();
	hud->GetCanvas()["apply"]->enable();
      }

      void		MenuConfiguration::Apply(ctvty::component::Hud*)
      {
	ctvty::Application::Assets().GetAsset("saves/configurations.json").Save(configuration);
      }

      void		MenuConfiguration::OverExit(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["exit overed"]->enable();
	hud->GetCanvas()["exit"]->disable();
      }

      void		MenuConfiguration::OnOverExit(ctvty::component::Hud* hud)
      {
	hud->GetCanvas()["exit overed"]->disable();
	hud->GetCanvas()["exit"]->enable();
      }

      void		MenuConfiguration::Exit(ctvty::component::Hud*)
      {
	ctvty::Application::LoadScene("options selection");
      }

    };
};
