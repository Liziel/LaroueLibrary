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
    Renderer&					_renderer;
    gdl::Geometry				_geometry;
    
  private:
    std::shared_ptr<ctvty::rendering::Texture>	_texture;
    ctvstd::Optional<std::string>		_text;

  public:
    Hud(Renderer&);

  public:
    void	SetTexture(std::shared_ptr<ctvty::rendering::Texture>&) final;
    void	SetText(const std::string&) final;

  private:
    std::shared_ptr<ctvty::rendering::Camera>	_associated;
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
    space	_space;
  public:
    inline
    space	getSpace() { return _space; }


  private:
    ctvty::utils::Quaternion	_rotation;
    ctvty::utils::Vector3D	_position;

  private:
    int				_level;

  public:
    inline
    int				GetLevel() { return _level; }

  public:
    void			Draw();

  private:
    bool			enabled;
  public:
    void		Enable() final;
    void		Disable() final;
    inline
    bool		state() { return enabled; }
  };
};

#endif

