#include "ctvty/component/camera.hh"
#include "ctvty/rendering/renderer.hh"
#include "ctvty/application.hh"
#include "ctvty/component/transform.hh"

REGISTER_FOR_SERIALIZATION(ctvty::component, Camera);
REGISTER_FOR_SERIALIZATION(ctvty::component::Camera, View);

namespace ctvty {
  namespace component {

    Camera::	Camera()
      : MonoBehaviour(nullptr, "Camera") {}

    Camera::	Camera(const serialization::Archive& __serial)
      : MonoBehaviour(nullptr, "Camera") {
      

      //first viewport
      if (__serial.exist("viewport")) {
	__serial["viewport"] & view;
	vtype = view::defined;
      } else if (__serial.exist("viewport id")) {
	__serial["viewport id"] & priority;
	vtype = view::automatic;
      } else {
	view.reset(new View(0,0, 0, 0));
	vtype = view::global;
      }

      //then offset
      if (__serial.exist("offset"))
	__serial["offset"] & offset;
      else
	offset.reset(new ctvty::utils::Vector3D(ctvty::utils::Vector3D::zero));

      //then look At
      if (__serial.exist("free look")) {
	__serial["free look"] & lookAt;
	ltype = look::free;
      } else if (__serial.exist("locked look")) {
	__serial["locked look"] & lookAt;
	ltype = look::locked;
      } else if (__serial.exist("forward look")) {
	if (!__serial["forward look"].is<bool>())
	  __serial["forward look"] & lookAt;
	else
	  lookAt.reset(new ctvty::utils::Vector3D(ctvty::utils::Vector3D::forward));
	ltype = look::forward;
      }

    }

    void	Camera::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::component::Camera, __serial_instance);
      
      if (vtype == view::defined) {
	__serial["viewport"] & view;
      } else if (vtype == view::automatic) {
	__serial["viewport id"] & priority;
      }

      __serial["offset"] & offset;

      if (ltype == look::free)
	__serial["free look"] & lookAt;
      else if (ltype == look::locked)
	__serial["locked look"] & lookAt;
      else
	__serial["forward look"] & lookAt;
    }


    Object*	Camera::clone() const {
      serialization::Serial	copy;
      copy & this;
      return copy.as<Camera*>();
    }


    void	Camera::Awake() {
      camera.reset(ctvty::rendering::Renderer::GetRenderer().CreateCamera());
      if (vtype == view::defined)
	camera->SetViewPort(view->x, view->y, view->width, view->height);
      else if (vtype == view::automatic)
	camera->DetectViewPort(priority);
    }

    void	Camera::Update() {
      if (vtype == view::global) {
	view->width = ctvty::Application::WindowWidth();
	view->height = ctvty::Application::WindowHeight();	
	camera->SetViewPort(view->x, view->y, view->width, view->height);
      }
      ctvty::utils::Vector3D _position = 
	transform->GetPosition() + *offset;
      ctvty::utils::Quaternion _rotation =
	ctvty::utils::Quaternion::identity;
      ctvty::utils::Vector3D _lookAt;

      if (ltype == look::forward) {
	_rotation = transform->GetRotation();
	_lookAt   = _rotation.RotatedVector(*lookAt) + _position;
      } else if (ltype == look::locked) {
	_lookAt	  = *lookAt;
      } else if (ltype == look::free) {
	_lookAt   = *lookAt + _position;
      }

      camera->SetCameraPosition(_position, _lookAt, _rotation);
    }


    Camera::View::	View(std::size_t a, std::size_t b, std::size_t c, std::size_t d)
      : x(a), y(b), width(c), height(d) {}

    Camera::View::	View(const serialization::Archive& __serial) {
      __serial["x"] & x;
      __serial["y"] & y;
      __serial["width"] & width;
      __serial["height"] & height;
    }

    void		Camera::View::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(Camera::View::Serialize, __serial_instance);
      __serial["x"] & x;
      __serial["y"] & y;
      __serial["width"] & width;
      __serial["height"] & height;
    }
  };
};
