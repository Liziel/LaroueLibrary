#ifndef MenuPrincipal_hh__
# define MenuPrincipal_hh__

#include "ctvty/monobehaviour.hpp"
#include "ctvty/component/Hud.hh"

namespace user_defined {
  namespace component {

    class MenuPrincipal : public MonoBehaviour<MenuPrincipal>{
    public:
			MenuPrincipal(const serialization::Archive&);
      void		Serialize(serialization::Archive&) const;

    public:
      void		OverExit(ctvty::component::Hud*);
    };

  };
};

#endif
