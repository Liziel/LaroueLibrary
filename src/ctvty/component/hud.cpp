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

    Hud::		Hud(const serialization::Archive& __serial) : overed(false) {

      onClickEnabled = false;
      if (__serial.exist("onClickEvent")) {
	onClickEnabled = true;
	__serial["onClickEvent"] & onClickEvent;
      }

      onOverEnabled = false;
      if (__serial.exist("onOverEvent")) {
	onOverEnabled = true;
	__serial["onOverEvent"] & onOverEvent;
      }

      onExitOverEnabled = false;
      if (__serial.exist("onExitOverEvent")) {
	onExitOverEnabled = true;
	__serial["onExitOverEvent"] & onExitOverEvent;
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
      if (__serial.exist("enabled")) {
	__serial["enabled"]	& enabled;
      }
    }

    void		Hud::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::component::Hud, __serial_instance);
      if (onClickEnabled) {
	__serial["onClickEvent"] & onClickEvent;
      }

      if (onOverEnabled) {
	__serial["onOverEvent"] & onOverEvent;
      }

      if (onExitOverEnabled) {
	__serial["onExitOverEvent"] & onExitOverEvent;
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
      if (enabled) {
	model->Enable();
      } else {
	model->Disable();
      }
    }

    void		Hud::SetText(const std::string& _text) {
      text = _text;
      model->SetText(text, police->GetShared(), r, g, b);
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
      if (enabled)
	model->Enable();
      else
	model->Disable();
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

      for (auto& pair : childrens)
	pair.second->SetParent(this);

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
      std::cout << "childrens count " << childrens.size() << std::endl;
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
	    if (_camera->Name() && *(_camera->Name()) == RenderCamera && _camera->Enabled()) {
	      camera = _camera;
	      std::cout << "fetch" << _camera << std::endl;
	    }
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
	children.second->SetName(children.first);
      }
    }

    void		Canvas::OnGui() {
      const std::shared_ptr<Event>	e ( ctvty::Event::current() );

      if (e->type() == Event::Type::mousemotion) {
	for (auto& children : childrens) {
	  if (children.second->state() && children.second->GetModel()->IsInside(e->position().x, e->position().y)) {
	    if (children.second->isOverable() && !children.second->overed)
	      BroadcastMessage(children.second->onOver(), children.second.get());
	    children.second->overed = true;
	  } else if (children.second->state() && children.second->overed && children.second->isExitOverable()) {
	    children.second->overed = false;
	    BroadcastMessage(children.second->onExitOver(), children.second.get());
	  }
	  if (children.second->GetModel()->IsInside(e->position().x, e->position().y))
	    children.second->overed = true;
	  else
	    children.second->overed = false;
	}
      } else if (e->type() == Event::Type::mousebuttondown) {
	for (auto& children : childrens) {
	  if (children.second->state() && children.second->isClickable()
	      && children.second->GetModel()->IsInside(e->position().x, e->position().y)) {
	    BroadcastMessage(children.second->onClick(), children.second.get());
	  }
	}
      }
    }
  };
};
