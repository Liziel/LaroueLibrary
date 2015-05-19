#ifndef HudComponent_hh__
# define HudComponent_hh__

# include <string>
# include <memory>
# include <map>

# include "ctvty/rendering/texture.hh"
# include "ctvty/rendering/hud.hh"
# include "ctvty/monobehviour.hpp"

namespace ctvty {
  namespace component {

    class Texture : public serialization::Serializable {
    private:
      std::string					path;
      std::shared_ptr<ctvty::rendering::Texture>	texture;

    public:
      inline
      ctvty::rendering::Texture&			GetTexture() {
	return *texture;
      }

    public:
      void		Serialize(serialization::Archive&) const;
			Texture(const serialization::Archive&);
    };
    
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
    public:
      void		SetText(const std::string&);

    private:
      std::shared_ptr<Texture>
			texture;

    private:
      std::shared_ptr<ctvty::rendering::Hud>
			model;

    public:
      float		sizex;
      float		sizey;

    public:
      void		Serialize(serialization::Archive&) const;
			Hud(const serialization::Archive&);
    };


    class Canvas : public Monobehaviour<Canvas> {
    private:
      std::map<std::string, Hud>	components;

    public:
      float				sizex;
      float				sizey;

    public:
      void				OnGui();
      
    public:
      void		Serialiaze(serialization::Archive&) const;
			Canvas(const serialization::Archive&);
    }
  };
};

#endif
