#include <cstdlib>
#include <algorithm>
#include "gdlimpl/renderer.hh"
#include "gdlimpl/camera.hh"

namespace GdlImpl {
  Camera::	Camera(Renderer& p)
    : renderer(p), rotation(ctvty::utils::Quaternion::identity), autoViewPort(false), priority(-1) {}

  Camera::	~Camera() {
    renderer.UnregisterCamera(this);
  }

  void		Camera::SetCameraPosition(const ctvty::utils::Vector3D& _position,
				  const ctvty::utils::Vector3D& _lookAt,
				  const ctvty::utils::Quaternion& _rotation) {
    position = _position;
    lookAt = _lookAt;
    rotation = _rotation;
  }


  void			Camera::SetViewPort(std::size_t baseX, std::size_t baseY,
				    std::size_t width, std::size_t height) {
    view.baseX = baseX;
    view.baseY = baseY;
    view.width = width;
    view.height = height;
    autoViewPort = false;
  }

  void			Camera::DetectViewPort(int _priority) {
    autoViewPort = true;
    priority = _priority;
  }

  void			Camera::DetectViewPort() {
    if (!autoViewPort)
      return ;

    std::size_t priority_cameras = 0;
    std::for_each(renderer.Cameras().begin(), renderer.Cameras().end(),
		  [&priority_cameras] (const Camera* c) { if (c->autoViewPort) ++priority_cameras; });

    switch (priority_cameras) {
    case 1:
      SetViewPort(0, 0, renderer.GetWidth(), renderer.GetHeight());
      break;
    case 2:
      if (priority != 1)
	SetViewPort(0, 0, renderer.GetWidth(), renderer.GetHeight() / 2);
      else
	SetViewPort(0, renderer.GetHeight() / 2, renderer.GetWidth(), renderer.GetHeight() / 2);
      break;
    case 3:
    case 4:
      switch (priority) {
      case 1 :
	SetViewPort(0, renderer.GetHeight() / 2,
		    renderer.GetWidth() / 2, renderer.GetHeight() / 2);
	break;
      case 2 :
	SetViewPort(renderer.GetWidth() / 2, renderer.GetHeight() / 2,
		    renderer.GetWidth() / 2, renderer.GetHeight() / 2);
	break;
      case 3:
	SetViewPort(0, 0,
		    renderer.GetWidth() / 2, renderer.GetHeight() / 2);
	break;
      default:
	SetViewPort(renderer.GetWidth() / 2, 0,
		    renderer.GetWidth() / 2, renderer.GetHeight() / 2);
	break;
      }
      break;
    }
    autoViewPort = true;
  }

  Camera::ViewPort&	Camera::GetViewPort() {
    return view;
  }

};
