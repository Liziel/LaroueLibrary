#ifndef MenuInGame_hh__
# define MenuInGame_hh__

#include "ctvty/monobehaviour.hpp"
#include "ctvty/component/hud.hh"

namespace user_defined {
  namespace component {

    class MenuInGame : public ctvty::MonoBehaviour<MenuInGame>{
    public:
			MenuInGame(const serialization::Archive&);
      void		Serialize(serialization::Archive&) const;

    public:
      void		OnDestroy();

      void		Play(ctvty::component::Hud*);
      void		OverPlay(ctvty::component::Hud*);
      void		OnOverPlay(ctvty::component::Hud*);

      void		OverExit(ctvty::component::Hud*);
      void		OnOverExit(ctvty::component::Hud*);
      void		Exit(ctvty::component::Hud*);
    };

  };
};

#endif
