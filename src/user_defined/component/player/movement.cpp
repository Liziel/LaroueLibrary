#include "user_defined/component/player/movement.hh"
#include "ctvty/input.hh"
#include "ctvty/debug.hpp"
#include "ctvty/component/transform.hh"
#include <cmath>

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
	if (!ctvty::Input::GetKeyState("left " + gameObject->Name())
	    && !ctvty::Input::GetKeyState("right " + gameObject->Name())
	    && !ctvty::Input::GetKeyState("up " + gameObject->Name())
	    && !ctvty::Input::GetKeyState("down " + gameObject->Name())) {
	  rb->GetVelocity()
	    = 
	    ctvty::utils::Vector3D(0, rb->GetVelocity().y, 0); 
	  return ;
	}
	float	rotate = 0.0;
	player_representation->GetTransformation()->GetRotation()
	  = ctvty::utils::Quaternion::Euler(ctvty::utils::Vector3D::up
					    * (ctvty::Input::GetKeyState("right " + gameObject->Name()) ? -1 : 1) * std::acos(ctvty::utils::Vector3D(ctvty::Input::GetKeyState("left " + gameObject->Name())
									       - ctvty::Input::GetKeyState("right " + gameObject->Name()),
									       0,
									       ctvty::Input::GetKeyState("up " + gameObject->Name())
									       - ctvty::Input::GetKeyState("down " + gameObject->Name())).GetNormalized()
							.DotProduct(ctvty::utils::Vector3D::forward)));
	rb->GetVelocity()
	  = 
	  ctvty::utils::Vector3D(0, rb->GetVelocity().y, 0) + 
	  6.0 * ctvty::utils::Vector3D(ctvty::Input::GetKeyState("left " + gameObject->Name())
						- ctvty::Input::GetKeyState("right " + gameObject->Name()),
						0,
						ctvty::Input::GetKeyState("up " + gameObject->Name())
						- ctvty::Input::GetKeyState("down " + gameObject->Name())
						);
      }

      void			Movement::Awake() {
	rb = GetComponent<ctvty::component::RigidBody>();
	player_representation = gameObject->GetChildByName(gameObject->Name() + " representation");
      }
    };
  };
};
