#include "ctvty/component/hud.hh"
#include "ctvty/component/transform.hh"

#include "ctvty/component/camera.hh"

#include "ctvty/rendering/renderer.hh"

#include "ctvty/application.hh"
#include "ctvty/event.hh"

#include "ctvty/debug.hpp"

REGISTER_FOR_SERIALIZATION(ctvty::component, Hud);
REGISTER_FOR_SERIALIZATION(ctvty::component, Canvas);

namespace ctvty {
  namespace component {

    Hud::		Hud(const serialization::Archive& __serial) {

      onClickEnabled = false;
      if (__serial.exist("onClickEvent")) {
	onClickEnabled = true;
	__serial["onClickEvent"] & onClickEvent;
      }

      onHoverEnabled = false;
      if (__serial.exist("onHoverEvent")) {
	onHoverEnabled = true;
	__serial["onHoverEvent"] & onHoverEvent;
      }

      text_enabled = false;
      if (__serial.exist("text")) {
	text_enabled = true;
	__serial["text"] & text;
	__serial["police"] & police;
	__serial["red"] & r;
	__serial["green"] & g;
	__serial["blue"] & b;
      }

      texture.reset();
      if (__serial.exist("texture")) {
	__serial["texture"] & texture;
      }

      __serial["size x"]	& sizex;
      __serial["size y"]	& sizey;

      __serial["offset x"]	& offx;
      __serial["offset y"]	& offy;

      __serial["level"]		& level;
      
      enabled = false;
      if (__serial.exist("enabled"))
	__serial["enabled"]	& enabled;
    }

    void		Hud::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::component::Hud, __serial_instance);
      if (onClickEnabled) {
	__serial["onClickEvent"] & onClickEvent;
      }

      if (onHoverEnabled) {
	__serial["onHoverEvent"] & onHoverEvent;
      }
      
      if (text_enabled) {
	__serial["text"] & text;
	__serial["police"] & police;
	__serial["red"] & r;
	__serial["green"] & g;
	__serial["blue"] & b;
      }

      if (texture) {
	__serial["texture"]	& texture;
      }

      __serial["size x"]	& sizex;
      __serial["size y"]	& sizey;

      __serial["offset x"]	& offx;
      __serial["offset y"]	& offy;

      __serial["level"]		& level;

      __serial["enabled"]	& enabled;
    }

    void		Hud::genScreenModel(float canvas_sizeX, float canvas_sizeY,
					    float canvas_offX, float canvas_offY) {
      model = ctvty::rendering::Renderer::GetRenderer().CreateHud();
      if (texture) {
	texture->delayedInstantiation();
	if (*texture)
	  model->SetTexture(texture->GetShared());
      }
      if (text_enabled) {
	police->delayedInstantiation();
	model->SetText(text, police->GetShared(), r, g, b);
      }
      model->SetPosition(sizex * canvas_sizeX, sizey * canvas_sizeY,
			 offx * canvas_sizeX + canvas_offX, offy * canvas_sizeY + canvas_offY);
      model->SetScreenSpace(level);
      model->Enable();
    }

    void		Hud::genWorldModel(float canvas_sizeX, float canvas_sizeY,
					   const utils::Vector3D& position,
					   const utils::Quaternion& rotation) {
      model = ctvty::rendering::Renderer::GetRenderer().CreateHud();
      if (texture) {
	texture->delayedInstantiation();
	if (*texture)
	  model->SetTexture(texture->GetShared());
      }
      if (text_enabled) {
	model->SetText(text, police->GetShared(), r, g, b);
      }
      model->SetPosition(sizex * canvas_sizeX, sizey * canvas_sizeY,
			 offx * canvas_sizeX, offy * canvas_sizeY);
      model->SetWorldSpace(rotation,
			   position + rotation.RotatedVector(ctvty::utils::Vector3D::back) * level);
      model->Enable();
    }

    Canvas::		Canvas(const serialization::Archive& __serial)
      : MonoBehaviour<Canvas>(nullptr, "Canvas") {

      WorldSpaceDefinition	= (__serial.exist("WorldSpace"));
      ScreenSpaceDefinition	= (__serial.exist("ScreenSpace"));

      if (ScreenSpaceDefinition) {
	__serial["offset x"]	& offX;
	__serial["offset y"]	& offY;
      }

      __serial["size x"]	& sizeX;
      __serial["size y"]	& sizeY;

      __serial["childrens"]	& childrens;

      if (__serial.exist("Render Camera"))
	__serial["Render Camera"] & RenderCamera;
      else
	RenderCamera = "GuiCamera";
    }

    void		Canvas::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::component::Canvas, __serial_instance);

      __serial["offset x"]	& offX;
      __serial["offset y"]	& offY;

      __serial["size x"]	& sizeX;
      __serial["size y"]	& sizeY;

      __serial["childrens"]	& childrens;

      if (WorldSpaceDefinition)
	__serial["WorldSpace"] & true;
      else
	__serial["ScreenSpace"] & true;

      __serial["Render Camera"] & RenderCamera;
    }

    void		Canvas::Awake() {
      Camera*	camera = nullptr;
      if (RenderCamera != "GuiCamera")
	{
	  std::list<Camera*> cameras = Object::FindObjectsOfType<Camera>();
	  for (Camera* _camera : cameras)
	    if (_camera->Name() && *(_camera->Name()) == RenderCamera)
	      camera = _camera;
	}

      for (auto& children : childrens) {
	if (ScreenSpaceDefinition)
	  children.second->genScreenModel(sizeX, sizeY, offX, offY);
	else
	  children.second->genWorldModel(sizeX, sizeY,
					 transform->GetHierarchyPosition(),
					 transform->GetHierarchyRotation());
	if (camera)
	  children.second->GetModel()->Associate(camera->getCamera());	
      }
    }

    void		Canvas::OnGui() {
      const std::shared_ptr<Event>	e ( ctvty::Event::current() );

      if (e->type() == Event::Type::mousemotion) {
	for (auto& children : childrens) {
	  if (children.second->isHoverable()
	      && children.second->GetModel()->IsInside(e->position().x, e->position().y))
	    BroadcastMessage(children.second->onHover());
	}
      } else if (e->type() == Event::Type::mousebuttondown) {
	for (auto& children : childrens) {
	  if (children.second->isClickable()
	      && children.second->GetModel()->IsInside(e->position().x, e->position().y)) {
	    BroadcastMessage(children.second->onClick());
	  }
	}
      }
    }
  };
};
