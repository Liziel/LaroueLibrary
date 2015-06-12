#include "user_defined/component/player/movement.hh"
#include "ctvty/input.hh"
#include "ctvty/debug.hpp"
#include "ctvty/component/transform.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component::player, Movement);

namespace user_defined {
  namespace component {
    namespace player {
      Movement::		Movement( const serialization::Archive& )
	: ctvty::MonoBehaviour<Movement>{nullptr, "Player Movement"} {}

      void			Movement::Serialize(serialization::Archive& __serial_instance) const {
	SERIALIZE_OBJECT_AS(user_defined::component::player::Movement, __serial_instance);
	(void)__serial;
      }

      void			Movement::Update() {
	ctvty::debug::Log("right " + gameObject->Name());
	rb->GetVelocity()
	  = ctvty::utils::Vector3D(ctvty::Input::GetKeyState("right " + gameObject->Name())
				   - ctvty::Input::GetKeyState("left " + gameObject->Name()),
				   rb->GetVelocity().y,
				   ctvty::Input::GetKeyState("up " + gameObject->Name())
				   - ctvty::Input::GetKeyState("down " + gameObject->Name())
				   );
      }

      void			Movement::Awake() {
	rb = GetComponent<ctvty::component::RigidBody>();
      }
    };
  };
};
