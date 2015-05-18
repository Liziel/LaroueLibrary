#include "ctvty/component/hud.hh"

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

      
      
    }

  };
};
