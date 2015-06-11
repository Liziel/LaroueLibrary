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
