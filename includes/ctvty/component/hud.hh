#ifndef HudComponent_hh__
# define HudComponent_hh__

# include <string>
# include <memory>
# include <map>

# include "ctvty/rendering/texture.hh"
# include "ctvty/rendering/hud.hh"
# include "ctvty/monobehaviour.hpp"

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
      void		genModel();

    private:
      float		offx;
      float		offy;
      float		sizex;
      float		sizey;
    public:


    private:
      bool		enabled;
    public:
      

    public:
      void		Serialize(serialization::Archive&) const;
			Hud(const serialization::Archive&);
    };

    class Canvas : public MonoBehaviour<Canvas> {
    private:
      std::map<std::string, std::unique_ptr<Hud*> >	childrens;

    private:
      bool				WorldSpaceDefinition;
      bool				ScreenSpaceDefinition;

    private:
      std::unique_ptr<utils::Vector3D>	WorldSpaceNormal;

    private:
      float				offX, offY;
    private:
      float				sizeX, sizeY;

      /*
       * define the viewport it's affiched in
       * in case its world space defined will also be rendered by the said camera 
       */
    private:
      std::string			RenderCamera;

    public:
      void				Awake();
      void				OnGui();
      
    public:
      void		Serialize(serialization::Archive&) const override;
			Canvas(const serialization::Archive&);
      Object*		clone() const override;
    };

  };
};

#endif
