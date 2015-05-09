#include "ctvty/component/camera.hh"
#include "ctvty/rendering/camera.hh"

REGISTER_FOR_SERIALIZATION(ctvty::component, Camera);

namespace ctvty {
  namespace component {

    Camera::	Camera()
      : MonoBehaviour(nullptr, "Camera") {}

    Camera::	Camera(const serialization::Archive& __serial)
      : MonoBehaviour(nullptr, "Camera") {

      lookForward = false;
      if (__serial.exist("lookAt"))
	__serial["lookAt"] & lookAt;
      else
	lookForward = true;

      if (__serial.exist("offset"))
	__serial["offset"] & offset;
      else
	offset = ctvty::utils::Vector3D::zero;
    }

    void	Camera::Serialize(serialization::Archive& __serial_intance) {
      SERIALIZE_OBJECT_AS(ctvty::component::Camera, __serial_instance);
      if (!lookForward)
	__serial["lookAt"] & lookAt;
      __serial["offset"] & offset;
    }


    Object*	Camera::Clone() const {
      serialization::Serial	copy;
      copy & this;
      return copy.as<Camera*>();
    }


    void	Camera::Awake() const {
      camera = ctvty::rendering::Renderer::GetRenderer().CreateCamera();
    }
  };
};
