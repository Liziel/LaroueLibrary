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

      texture.reset();
      if (__serial.exist("texture")) {
	__serial["texture"] & texture;
      }

      __serial["offset x"]	& offx;
      __serial["offset y"]	& offy;

      __serial["offset x"]	& offx;
      __serial["offset y"]	& offy;

      __serial["enabled"]	& enabled;
    }


    Canvas::		Canvas(const serialization::Archive& __serial)
      : MonoBehaviour<Canvas>(nullptr, "Canvas") {
      WorldSpaceDefinition	= __serial.exist("WorldSpace") && __serial["WorldSpace"].as<bool>();
      ScreenSpaceDefinition	= __serial.exist("ScreenSpace") && __serial["ScreenSpace"].as<bool>();

      if (WorldSpaceDefinition) {
	__serial["WorldSpace normal"] & WorldSpaceNormal;
      }

      __serial["offset x"]	& offX;
      __serial["offset y"]	& offY;

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
  };
};
