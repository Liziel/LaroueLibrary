#include "ctvty/component/hud.hh"
#include "ctvty/component/camera.hh"
#include "ctvty/rendering/camera.hh"
#include "ctvty/rendering/renderer.hh"

REGISTER_FOR_SERIALIZATION(ctvty::component, Texture);
REGISTER_FOR_SERIALIZATION(ctvty::component, Hud);
REGISTER_FOR_SERIALIZATION(ctvty::component, Canvas);

namespace ctvty {
  namespace component {

    Texture::		Texture(const serialization::Archive& __serial) {
      __serial["path"] & path;
      texture.reset(ctvty::rendering::Renderer::GetRenderer().LoadTexture(path));
    }

    void		Texture::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::component::Texture, __serial_instance);
      __serial["path"] & path;
    }


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

      texture_path.rese();
      texture_path.reset();
      if (__serial.exist("texture")) {
	__serial["texture"] & texture_path;
      }

      __serial["offset x"]	& offx;
      __serial["offset y"]	& offy;

      __serial["offset x"]	& offx;
      __serial["offset y"]	& offy;

      __serial["level"]		& level;

      __serial["enabled"]	& enabled;
    }

    void		Hud::Serialize(serializatino::Archive& __serial) const {
      if (onClickEnabled) {
	__serial["onClickEvent"] & onClickEvent;
      }

      if (onHoverEnabled) {
	__serial["onHoverEvent"] & onHoverEvent;
      }
      
      if (text_enabled) {
	__serial["text"] & text;
      }

      if (texture_path) {
	__serial["texture"]	& texture_path;
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

    void		Hud::genScreenModel(float canvas_sizeX, float canvas_sizeY,
					    float canvas_offX, float canvas_sizeX) {
      model.reset(ctvty::rendering::Renderer::GetRenderer().CreateHud());
      if (texture_path) {
	texture.reset(ctvty::rendering::Renderer::GetRenderer().LoadTexture(*texture_path));
	model->SetTexture();
      }
      if (text_enabled) {
	model->SetText(text);
      }
      model->SetPosition(sizex * canvas_sizeX, sizey * canvasSizeY,
			 offx * canvas_sizeX + canvas_offX, offx * canvas_sizeX + canvas_offX);
      model->SetScreenSpace(level);
    }

    void		Hud::genWorldModel(float canvas_sizeX, float canvas_sizeY,
					    float canvas_offX, float canvas_sizeX) {
      model.reset(ctvty::rendering::Renderer::GetRenderer().CreateHud());
      if (texture_path) {
	texture.reset(ctvty::rendering::Renderer::GetRenderer().LoadTexture(*texture_path));
	model->SetTexture();
      }
      if (text_enabled) {
	model->SetText(text);
      }
      model->SetPosition(sizex * canvas_sizeX, sizey * canvasSizeY,
			 offx * canvas_sizeX + canvas_offX, offx * canvas_sizeX + canvas_offX);
      model->SetWorldModel(transform->GetRotation(),
			   trnasform->GetPosition());
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
      for (auto children : childrens) {
	if (WoldSpaceDefinition)
	  children.second->genScreenModel(sizeX, sizeY, offx, offy);
	else
	  children.second->genWorldMoel(sizeX, sizeY);
      }
    }
  };
};
