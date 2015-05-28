#ifndef GdlHudImpl_hh__
# define GdlHudImpl_hh__

# include <memory>

# include "Geometry.hh"
# include "std/optional.hpp"

# include "ctvty/rendering/hud.hh"

namespace GdlImpl {

  class Renderer;
  class Hud : public ctvty::rendering::Hud {

  private:
    Renderer& _renderer;
    gdl::Geometry _geometry;
    bool	geoInit;
    std::list< std::weak_ptr<Hud> >*	_self_container;
    std::weak_ptr< Hud >		_self;
  public:
			Hud(Renderer& renderer ,
			    std::list< std::weak_ptr<Hud> >* self_container);
    inline void		SetWeakRef(std::weak_ptr<Hud> self ) { _self = self; }

  private:
    std::shared_ptr<ctvty::rendering::Texture>		_texture;
    ctvstd::Optional<std::string>			_text;
    std::shared_ptr<ctvty::asset::Police::Wrapper>	_police;
    SDL_Color						color;
    SDL_Surface*					texture;

  public:
    void	SetTexture(std::shared_ptr<ctvty::rendering::Texture>&) final;
    void	SetText(const std::string&, std::shared_ptr<ctvty::asset::Police::Wrapper>, int, int, int) final;

  private:
    std::shared_ptr<ctvty::rendering::Camera> _associated;
  public:
    inline
    std::shared_ptr<ctvty::rendering::Camera>
		GetAssociatedCamera() {return _associated; }
    void	Associate(const std::shared_ptr<ctvty::rendering::Camera>&) final;

  private:
    float	_offx, _offy, _sizex, _sizey;
  public:
    void	SetPosition(float sizex, float sizey,
		    float offx = 0, float offy = 0) final;
  public:
    enum class space { none, screen, world };
    void	SetWorldSpace(const ctvty::utils::Quaternion& rotation,
		      const ctvty::utils::Vector3D& position) final;
    void	SetScreenSpace(int level) final;

  public:
    bool	IsInside(float screenX, float screenY) final;

  private:
    space _space;
  public:
    inline
    space	getSpace() { return _space; }
  private:
    ctvty::utils::Quaternion _rotation;
    ctvty::utils::Vector3D _position;

  private:
    int		_level;
  public:
    inline
    int		GetLevel() { return _level; }

  public:
    void	Draw();

  private:
    bool	enabled;
  public:
    void	Enable() final;
    void	Disable() final;
    inline
    bool	state() { return enabled; }
  };
};
#endif
