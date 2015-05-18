#ifndef HudComponent_hh__
# define HudComponent_hh__

# include <string>
# include <memory>
# include <map>

# include "ctvty/rendering/hud.hh"
# include "ctvty/monobehviour.hpp"

namespace ctvty {
  namespace component {
    
    class Hud : public serialization::Serializable {
    private:
      bool		onClickEnabled;
      std::string	onClickEvent;
    public:
      inline
      bool			isClickable() { return onClickEnabled; }
      inline
      const std::string&	onClick() { return onClickEvent; }

    private:
      bool		onHoverEnabled;
      std::string	onHoverEvent;
    public:
      inline
      bool			isHoverable() { return onClickEnabled; }
      inline
      const std::string&	onHover() { return onClickEvent; }      

    private:
      bool		text_enabled;
      std::string	text;

    private:
      bool		texture_enabled;
      std::string	path;

    private:
      std::shared_ptr<ctvty::rendering::Hud>
			model;

    public:
      void		Serialize(serialization::Archive&) const;
			Hud(const serialization::Archive&);
    };


    class Canvas : public Monobehaviour<Canvas> {
    private:
      std::map<std::string, Hud>	components;

    public:
      void				OnGui();
      
    public:
      void		Serialiaze(serialization::Archive&) const;
			Canvas(const serialization::Archive&);
    }
  };
};

#endif
