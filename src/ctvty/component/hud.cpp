#include "ctvty/component/hud.hh"
#include "ctvty/component/transform.hh"

#include "ctvty/component/camera.hh"

#include "ctvty/rendering/renderer.hh"

#include "ctvty/application.hh"
#include "ctvty/event.hh"

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
      }

      texture.reset();
      if (__serial.exist("texture")) {
	__serial["texture"] & texture;
      }

      __serial["offset x"]	& offx;
      __serial["offset y"]	& offy;

      if (__serial.exist("offset x"))
	__serial["offset x"]	& offx;
      else
	offx = 0.f;
      if (__serial.exist("offset y"))
	__serial["offset y"]	& offy;
      else
	offy = 0.f;

      __serial["level"]		& level;

      __serial["enabled"]	& enabled;
    }

    void		Hud::Serialize(serialization::Archive& __serial) const {
      if (onClickEnabled) {
	__serial["onClickEvent"] & onClickEvent;
      }

      if (onHoverEnabled) {
	__serial["onHoverEvent"] & onHoverEvent;
      }
      
      if (text_enabled) {
	__serial["text"] & text;
      }

      if (texture) {
	__serial["texture"]	& texture;
      }

      __serial["offset x"]	& offx;
      __serial["offset y"]	& offy;

      __serial["offset x"]	& offx;
      __serial["offset y"]	& offy;

      __serial["level"]		& level;
      __serial["enabled"]	& enabled;
    }

    void		Hud::genScreenModel(float canvas_sizeX, float canvas_sizeY,
					    float canvas_offX, float canvas_offY) {
      model.reset(ctvty::rendering::Renderer::GetRenderer().CreateHud());
      if (texture) {
	texture->delayedInstantiation();
	model->SetTexture(texture->GetTexture());
      }
      if (text_enabled) {
	model->SetText(text);
      }
      model->SetPosition(sizex * canvas_sizeX, sizey * canvas_sizeY,
			 offx * canvas_sizeX + canvas_offX, offy * canvas_sizeY + canvas_offY);
      model->SetScreenSpace(level);
    }

    void		Hud::genWorldModel(float canvas_sizeX, float canvas_sizeY,
					   const utils::Vector3D& position,
					   const utils::Quaternion& rotation) {
      model.reset(ctvty::rendering::Renderer::GetRenderer().CreateHud());
      if (texture) {
	texture->delayedInstantiation();
	model->SetTexture(texture->GetTexture());
      }
      if (text_enabled) {
	model->SetText(text);
      }
      model->SetPosition(sizex * canvas_sizeX, sizey * canvas_sizeY);
      model->SetWorldSpace(rotation,
			   position + rotation.RotatedVector(ctvty::utils::Vector3D::back) * level);
    }

    Canvas::		Canvas(const serialization::Archive& __serial)
      : MonoBehaviour<Canvas>(nullptr, "Canvas") {

      WorldSpaceDefinition	= __serial.exist("WorldSpace") && __serial["WorldSpace"].as<bool>();
      ScreenSpaceDefinition	= __serial.exist("ScreenSpace") && __serial["ScreenSpace"].as<bool>();

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

    Object*		Canvas::clone() const {
      return new Canvas{ serialization::Archive(this) };
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
	if (WorldSpaceDefinition)
	  children.second->genScreenModel(sizeX, sizeY, offX, offY);
	else
	  children.second->genWorldModel(sizeX, sizeY,
					 transform->GetPosition(),
					 transform->GetRotation());
	children.second->GetModel()->Associate(camera->getCamera());	
      }
    }

    void		Canvas::OnGui() {
      const std::shared_ptr<Event>	e ( ctvty::Event::current() );

      if (e->type() == Event::Type::mousemotion) {
	for (auto& children : childrens)
	  if (children.second->isHoverable()
	      && children.second->GetModel()->IsInside(e->position().x, e->position().y))
	    BroadcastMessage(children.second->onHover());
	    
      } else if (e->type() == Event::Type::mousebuttondown) {
	for (auto& children : childrens)
	  if (children.second->isClickable()
	      && children.second->GetModel()->IsInside(e->position().x, e->position().y))
	    BroadcastMessage(children.second->onClick());
      }
    }
  };
};
