#ifndef MENUOPTIONS_HH_
# define MENUOPTIONS_HH_

#include "ctvty/monobehaviour.hpp"
#include "ctvty/component/hud.hh"

namespace user_defined
{
  namespace component
  {
    class MenuOptions : public ctvty::MonoBehaviour<MenuOptions>
    {
    public:
      MenuOptions(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

    public:
      void		OverExit(ctvty::component::Hud*);
      void		OnOverExit(ctvty::component::Hud*);
      void		Exit(ctvty::component::Hud*);
    };
  };
};

#endif
