#ifndef MenuConfiguration_hh__
# define MenuConfiguration_hh__

# include "ctvty/monobehaviour.hpp"
# include "ctvty/component/hud.hh"
# include "user_defined/component/configurations.hh"


namespace user_defined
{
  namespace component
  {

    class MenuConfiguration : public ctvty::MonoBehaviour<MenuConfiguration>
    {
    private:
      std::shared_ptr<ctvty::asset::Texture>	texture;
      std::shared_ptr<Configuration>		configuration;

    public:
      MenuConfiguration(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

    public:
      void		Awake();

      void		OverApply(ctvty::component::Hud*);
      void		OnOverApply(ctvty::component::Hud*);
      void	        Apply(ctvty::component::Hud*);

      void	        UpSizeX(ctvty::component::Hud*);
      void	        DownSizeX(ctvty::component::Hud*);

      void	        UpSizeY(ctvty::component::Hud*);
      void	        DownSizeY(ctvty::component::Hud*);

      void	        PlayerClick1(ctvty::component::Hud*);
      void	        PlayerClick2(ctvty::component::Hud*);

      void	        IAClick1(ctvty::component::Hud*);
      void	        IAClick2(ctvty::component::Hud*);

      void		OverExit(ctvty::component::Hud*);
      void		OnOverExit(ctvty::component::Hud*);
      void		Exit(ctvty::component::Hud*);
    };
  };
};

#endif
