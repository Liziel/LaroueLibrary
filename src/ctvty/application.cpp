#include "ctvty/event/clock.hh"
#include "ctvty/application.hh"

namespace ctvty {

  REGISTER_FOR_SERIALIZATION(ctvty, Application);
  REGISTER_FOR_SERIALIZATION(ctvty::Application, Scene);

  Application::		Application(const serialization::Archive& __serial)
    : leave_state(false), delayed_quit(nullptr) {
    float	fps;

    __serial["scenes"] & scenes;
    __serial["load"] & load_scene;
    if (__serial.exist("fps"))
      __serial["fps"] & fps;
    else
      fps = 30;
    ctvty::event::Clock::GetClock().SetMaxFramePerSeconds(fps);
  }

  void			Application::Serialize(serialization::Archive& __serial_instance) {
    float	fps = 30;
    SERIALIZE_OBJECT_AS(ctvty::Application, __serial_instance);
    __serial["scenes"] & scenes;
    __serial["fps"] & fps;
    __serial["load"] & load_scene;
  }

  bool			Application::Initialize(const std::string& location) {
    Application*&		app = GetApplication();
    ctvty::asset::Assets*	assets = new ctvty::asset::Assets(location);
    serialization::Serial*	serial;

    {//serialization asset function
      serialization::StoreFunction("asset",
				   [&assets] (const serialization::Serial& __serial) -> serialization::Serial* {
	  std::string path;
	  __serial & path;
	  
	  serialization::Serial* product =
	    serialization::Serial::InstantiateFromFile(assets->GetAsset(path).GetFile().GetPath());
	  return product;
	});
    }

    {//application deserialization
      if (!(assets->GetAsset("application.json").GetFile())) {
	std::cerr << "application error: application.json not found, the application will quit" << std::endl;
	return false;
      }
      serial = serialization::Serial::InstantiateFromFile(assets->GetAsset("application.json")
							  .GetFile().GetPath());
      app = (Application*)serialization::Serializable::Instantiate(*serial);
      app->assets = assets;
      delete serial;
    }

    return true;
  }

  asset::Assets&	Application::Assets() { return *(GetApplication()->assets); }

  Application*&		Application::GetApplication() { static Application* app = nullptr; return app; }

  Application::Scene::	Scene(const serialization::Archive& __serial) {
    __serial["objects"] & objects;
    __serial["name"] & name;
  }

  void			Application::Scene::Serialize(serialization::Archive& __serial_instance) {
    SERIALIZE_OBJECT_AS(ctvty::Application::Scene, __serial_instance);
    __serial["objects"] & objects;
    __serial["name"] & name;
  }

};
