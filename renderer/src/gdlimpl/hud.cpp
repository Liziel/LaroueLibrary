#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

#include "gdlimpl/hud.hh"
#include "gdlimpl/renderer.hh"

namespace GdlImpl {
  Hud::			Hud(Renderer& renderer, std::list< std::weak_ptr<Hud> >* l)
    : _renderer(renderer),
      _self_container(l),
      _rotation(ctvty::utils::Quaternion::identity) {

    _geometry.pushVertex(glm::vec3(0,	0, 0));
    _geometry.pushVertex(glm::vec3(1.f, 0, 0));
    _geometry.pushVertex(glm::vec3(1.f, 1.f, 0));
    _geometry.pushVertex(glm::vec3(0,	1.f, 0));

    _geometry.pushUv(glm::vec2(0.0f, 1.0f)); 
    _geometry.pushUv(glm::vec2(1.0f, 1.0f)); 
    _geometry.pushUv(glm::vec2(1.0f, 0.0f)); 
    _geometry.pushUv(glm::vec2(0.0f, 0.0f)); 
    
    _geometry.build();
  }

  void			Hud::SetTexture(std::shared_ptr<ctvty::rendering::Texture>& texture) {
    _texture = texture;
  }

  void			Hud::SetText(const std::string& text,
				     std::shared_ptr<ctvty::asset::Police::Wrapper> police,
				     int r, int g, int b) {
    _text = text;
    _police = police;
    color = { r, g, b };
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

    if (_texture)
      _texture->Bind();
    if (_text) {
    }
    if (_space == space::screen) {
      transform = glm::translate(_offx * _renderer.GetWidth(), _offy * _renderer.GetHeight(), 0.f);
      transform *= glm::scale(_sizex * _renderer.GetWidth(), _sizey * _renderer.GetHeight(), 0.f);
    } else {
      
    }
    _geometry.draw(_renderer.GetShader(), transform, GL_QUADS);
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
