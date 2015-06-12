#include "user_defined/component/ladder.hh"
#include "ctvty/application.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, Ladder);

namespace user_defined {
  namespace component {

    Ladder::Ladder(const serialization::Archive& __serial)
      : ctvty::MonoBehaviour<Ladder>(nullptr, "Ladder") {
      __serial["path"] & path;
      RegisterListener("Scored", &Ladder::Scored);
    }

    void		Ladder::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::Ladder, __serial_instance);
      __serial["path"] & path;
    }

    void		Ladder::Awake()
    {
      log = ctvty::Application::Assets()
	.LoadAs<ScoreLog>(ctvty::Application::Assets().path() + path);
    }

    void		Ladder::OnDestroy()
    {
      if (log)
	{
	  ctvty::Application::Assets().GetAsset(ctvty::Application::Assets().path() + path).Save(log);
	}
    }

    void		Ladder::Scored(const std::string& player)
    {
      (*log)[player] += 1;
    }


    ScoreLog::ScoreLog(const serialization::Archive& __serial)
    {
      __serial["log"] & log;
    }

    void		ScoreLog::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::ScoreLog, __serial_instance);
      __serial["log"] & log;
    }

    int&	ScoreLog::operator[](const std::string& id)
    {
      return log[id];
    }
  };
};

namespace user_defined
{
  namespace component
  {
    MenuLadder::MenuLadder(const serialization::Archive&)
      : ctvty::Monobehaviour<MenuLadder>(nullptr, "MenuLadder")
    {
      RegisterListener("exit over", &MenuLadder::OverExit);
      RegisterListener("exit OnOver", &MenuLadder::OnOverExit);
      RegisterListener("exit click", &MenuLadder::Exit);
    }

    void	        MenuLadder::Serialize(serialization::Archive& __serial_instance) const
    {
      SERIALIZE_OBJECT_AS(user_defined::component::MenuLadder, __serial_instance);
      (void)__serial;
    }

    void		MenuLadder::OverExit(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["exit overed"]->enable();
      hud->GetCanvas()["exit"]->disable();
    }

    void		MenuLadder::OnOverExit(ctvty::component::Hud* hud)
    {
      hud->GetCanvas()["exit overed"]->disable();
      hud->GetCanvas()["exit"]->enable();
    }

    void		MenuLadder::Exit(ctvty::component::Hud*)
    {
      ctvty::Application::Quit();
    }
  };
};
