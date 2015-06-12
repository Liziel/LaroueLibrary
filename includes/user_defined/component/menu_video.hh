#ifndef MenuVideo_hh__
# define MenuVideo_hh__

#include "ctvty/monobehaviour.hpp"
#include "ctvty/component/hud.hh"

namespace user_defined
{
  namespace component
  {

    class MenuVideo : public ctvty::MonoBehaviour<MenuVideo>
    {
    private:
      int	_resolution;
      bool	_fullscreen;

    public:
      MenuVideo(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

    public:
      void		OverApply(ctvty::component::Hud*);
      void		OnOverApply(ctvty::component::Hud*);
      void	        Apply(ctvty::component::Hud*);

      void	        Resolution1(ctvty::component::Hud*);
      void	        Resolution2(ctvty::component::Hud*);
      void	        Resolution3(ctvty::component::Hud*);

      void	        OnClick(ctvty::component::Hud*);
      void	        OffClick(ctvty::component::Hud*);

      void		OverExit(ctvty::component::Hud*);
      void		OnOverExit(ctvty::component::Hud*);
      void		Exit(ctvty::component::Hud*);
    };
  };
};


#endif
