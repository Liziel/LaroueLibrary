#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

#include "gdlimpl/renderer.hh"
#include "gdlimpl/hud.hh"

namespace GdlImpl {
  Hud::			Hud(Renderer& renderer, std::list< std::weak_ptr<Hud> >* l)
    : _renderer(renderer),
      _self_container(l),
      _rotation(ctvty::utils::Quaternion::identity),
      texture(nullptr), geoInit(false) {
  }

  void			Hud::SetTexture(std::shared_ptr<ctvty::rendering::Texture>& texture) {
    _texture = texture;
    if (!geoInit) {
      _geometry.pushVertex(glm::vec3(0,	0, 0));
      _geometry.pushVertex(glm::vec3(1.f, 0, 0));
      _geometry.pushVertex(glm::vec3(1.f, 1.f, 0));
      _geometry.pushVertex(glm::vec3(0,	1.f, 0));

      _geometry.pushUv(glm::vec2(0.0f, 1.0f)); 
      _geometry.pushUv(glm::vec2(1.0f, 1.0f));
      _geometry.pushUv(glm::vec2(1.0f, 0.0f)); 
      _geometry.pushUv(glm::vec2(0.0f, 0.0f));
    
      _geometry.build();
      geoInit = true;
    }
  }

  void			Hud::SetText(const std::string& text,
				     std::shared_ptr<ctvty::asset::Police::Wrapper> police,
				     int r, int g, int b) {
    _text = text;
    _police = police;
    color = { static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b) };
    if (&(*police) == nullptr)
      return ;
    if (texture)
      SDL_FreeSurface(texture);
    texture = TTF_RenderText_Blended(&(*police), text.c_str(), color);
    if (!geoInit) {
      _geometry.pushVertex(glm::vec3(0,	0, 0));
      _geometry.pushVertex(glm::vec3(1.f, 0, 0));
      _geometry.pushVertex(glm::vec3(1.f, 1.f, 0));
      _geometry.pushVertex(glm::vec3(0,	1.f, 0));

      _geometry.pushUv(glm::vec2(0.0f, 0.0f)); 
      _geometry.pushUv(glm::vec2(1.0f, 0.0f));
      _geometry.pushUv(glm::vec2(1.0f, 1.0f)); 
      _geometry.pushUv(glm::vec2(0.0f, 1.0f));
    
      _geometry.build();
      geoInit = true;
    }
  }

  void			Hud::Associate(const std::shared_ptr<ctvty::rendering::Camera>& associated) {
    _associated = associated;
  }

  void			Hud::SetPosition(float sizex, float sizey,
					 float offx, float offy) {
    _sizex = sizex;
    _sizey = sizey;
    _offx = offx;
    _offy = offy;
  }

  void			Hud::SetWorldSpace(const ctvty::utils::Quaternion& rotation,
					   const ctvty::utils::Vector3D& position) {
    _rotation = rotation;
    _position = position;
    _space = space::world;
  }

  void			Hud::SetScreenSpace(int level) {
    _level = level;
    _space = space::screen;
    if (_self_container)
      _self_container->remove_if([this](std::weak_ptr<Hud> t) { return t.expired() || this == t.lock().get(); } );
    if (!_associated)
      _self_container = _renderer.AddScreenHud(_self);
  }

  void			Hud::Draw() {
    glm::mat4 transform;
    unsigned text_texture = 0;

    if (_texture)
      _texture->Bind();
    if (_text && texture) {
      glGenTextures(1, &text_texture);
      glBindTexture(GL_TEXTURE_2D, text_texture);

      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->w, texture->h, 0, GL_BGRA,
		   GL_UNSIGNED_BYTE, texture->pixels);
    }
    if (_space == space::screen) {
      transform = glm::translate(_offx * _renderer.GetWidth(), _offy * _renderer.GetHeight(), 0.f);
      transform *= glm::scale(_sizex * _renderer.GetWidth(), _sizey * _renderer.GetHeight(), 0.f);
    } else {
      
    }
    _geometry.draw(_renderer.GetShader(), transform, GL_QUADS);
    if (_text) {
      glDeleteTextures(1, &text_texture);
    }
  }

  void			Hud::Enable() {
    enabled = true;
  }

  void			Hud::Disable() {
    enabled = false;
  }

  bool			Hud::IsInside(float x, float y) {
    if (_space == space::screen) {
      return
	y > _offy * _renderer.GetHeight() &&
	y < _offy * _renderer.GetHeight()  + _sizey * _renderer.GetHeight() &&
	x > _offx * _renderer.GetWidth() &&
	x < _offx * _renderer.GetWidth() + _sizex * _renderer.GetWidth();
    }
    return false;
  }
};
