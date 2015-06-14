#include "user_defined/component/ladder.hh"
#include "ctvty/application.hh"
#include "ctvty/debug.hpp"

REGISTER_FOR_SERIALIZATION(user_defined::component, Ladder);
REGISTER_FOR_SERIALIZATION(user_defined::component, MenuLadder);
REGISTER_FOR_SERIALIZATION(user_defined::component, ScoreLog);

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
	.GetAsset(ctvty::Application::Assets().GetLocation() + path).LoadAs<ScoreLog>();
    }

    void		Ladder::OnDestroy()
    {
      if (log)
	{
	  ctvty::Application::Assets().GetAsset(ctvty::Application::Assets().GetLocation() + path).Save(log);
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
      : ctvty::MonoBehaviour<MenuLadder>(nullptr, "MenuLadder")
    {
      RegisterListener("exit over", &MenuLadder::OverExit);
      RegisterListener("exit OnOver", &MenuLadder::OnOverExit);
      RegisterListener("exit click", &MenuLadder::Exit);
    }

    void		MenuLadder::Awake() {
      ctvty::component::Canvas& canvas	= *GetComponent<ctvty::component::Canvas>();
      std::list<auto_sort>	sorter;

      log = ctvty::Application::Assets()
	.GetAsset("log/logLadder.json").LoadAs<ScoreLog>();
      ctvty::debug::Log(log);
      for (auto pair : *log)
	sorter.emplace_back(pair.first, pair.second);
      sorter.sort(std::greater<int>());
      std::size_t	i = 0;
      for (auto_sort& score : sorter) {
	if (i < 5) {
	  std::size_t	j = 0;
	  for (char c : (std::string)score) {
	    std::shared_ptr<ctvty::asset::Texture>	texture(new ctvty::asset::Texture(std::string("menu/textures/") + static_cast<char>(c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c) + ".json"));
	    canvas[std::string("letter ") + static_cast<char>(j + '0')
		   + " row " + static_cast<char>(i + '0')]->SetTexture(texture);
	    canvas[std::string("letter ") + static_cast<char>(j + '0')
		   + " row " + static_cast<char>(i + '0')]->enable();

	    ++j;
	    if (j == 3)
	      break;
	  }
	  {
	    std::shared_ptr<ctvty::asset::Texture>	texture(new ctvty::asset::Texture(std::string("menu/textures/") + static_cast<char>((int)score / 100 + '0') + ".json"));
	    canvas[std::string("number ") + '0'
		   + " row " + static_cast<char>(i + '0')]->SetTexture(texture);
	    canvas[std::string("number ") + '0'
		   + " row " + static_cast<char>(i + '0')]->enable();
	  }
	  {
	    std::shared_ptr<ctvty::asset::Texture>	texture(new ctvty::asset::Texture(std::string("menu/textures/") + static_cast<char>(((int)score / 10) % 10 + '0') + ".json"));
	    canvas[std::string("number ") + '1'
		   + " row " + static_cast<char>(i + '0')]->SetTexture(texture);
	    canvas[std::string("number ") + '1'
		   + " row " + static_cast<char>(i + '0')]->enable();
	  }
	  {
	    std::shared_ptr<ctvty::asset::Texture>	texture(new ctvty::asset::Texture(std::string("menu/textures/") + static_cast<char>((int)score % 10 + '0') + ".json"));
	    canvas[std::string("number ") + '2'
		   + " row " + static_cast<char>(i + '0')]->SetTexture(texture);
	    canvas[std::string("number ") + '2'
		   + " row " + static_cast<char>(i + '0')]->enable();
	  }
	} else
	  break;
	++i;
      }
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
      ctvty::Application::LoadScene("menu principal");
    }
  };
};
