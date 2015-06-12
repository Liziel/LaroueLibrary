#ifndef OptionsSelections_hh__
# define OptionsSelections_hh__

#include "ctvty/monobehaviour.hpp"
#include "ctvty/component/hud.hh"

namespace user_defined
{
  namespace component
  {

    class OptionsSelection : public ctvty::MonoBehaviour<OptionsSelection>
    {
    public:
      OptionsSelection(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

    public:
      void		OverVideo(ctvty::component::Hud*);
      void		OnOverVideo(ctvty::component::Hud*);
      void	        Video(ctvty::component::Hud*);

      void		OverConfiguration(ctvty::component::Hud*);
      void		OnOverConfiguration(ctvty::component::Hud*);

      void		OverShortcuts(ctvty::component::Hud*);
      void		OnOverShortcuts(ctvty::component::Hud*);
      void	        Shortcuts(ctvty::component::Hud*);

      void		OverExit(ctvty::component::Hud*);
      void		OnOverExit(ctvty::component::Hud*);
      void		Exit(ctvty::component::Hud*);
    };
  };
};

#endif
