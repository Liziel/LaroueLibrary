#ifndef MENUOPTIONS_HH_
# define MENUOPTIONS_HH_

# include "ctvty/monobehaviour.hpp"
# include "ctvty/component/hud.hh"
# include "ctvty/assets/texture.hh"

namespace user_defined
{
  namespace component
  {
    class MenuOptions : public ctvty::MonoBehaviour<MenuOptions>
    {
    private:
      bool					setting;
      ctvty::component::Hud*			setted;
      std::shared_ptr<ctvty::asset::Texture>	texture;

    public:
      MenuOptions(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

    public:
      void		OnSetterClick(ctvty::component::Hud*);
      void		OverExit(ctvty::component::Hud*);
      void		OnOverExit(ctvty::component::Hud*);
      void		Exit(ctvty::component::Hud*);

    public:
      void		OnGui();
      void		Awake();
    };
  };
};

#endif
