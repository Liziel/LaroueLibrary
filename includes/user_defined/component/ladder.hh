#ifndef LADDER_HH_
# define LADDER_HH_

#include "ctvty/monobehaviour.hpp"
#include "ctvty/component/hud.hh"

namespace user_defined
{
  namespace component
  {

    class ScoreLog : public serialization::Serializable
    {
    public:
      ScoreLog(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

    public:
      int& operator[](const std::string &);
      inline std::map<std::string, int>::iterator	begin() { return log.begin(); }
      inline std::map<std::string, int>::iterator	end() { return log.end(); }

    private:
      std::map<std::string, int>		log;
    };

    class Ladder : public ctvty::MonoBehaviour<Ladder>
    {
    public:
      Ladder(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

    public:
      void Awake();
      void OnDestroy();
      void Scored(const std::string&);

    private:
      std::string		path;
      std::shared_ptr<ScoreLog>	log;
    };
  };
};

namespace user_defined
{
  namespace component
  {
    class MenuLadder : public ctvty::MonoBehaviour<MenuLadder>
    {
    private:
      struct auto_sort {
	int		score;
	std::string	name;
	operator std::string() { return name; }
	operator int() { return score; }
	auto_sort(std::string _name, int _score) : score(_score), name(_name) {}
      };
      std::shared_ptr<ScoreLog>		log;
    public:
      MenuLadder(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;
      void	Awake();

    public:
      void	OverExit(ctvty::component::Hud*);
      void	OnOverExit(ctvty::component::Hud*);
      void	Exit(ctvty::component::Hud*);
    };
  };
};

#endif
