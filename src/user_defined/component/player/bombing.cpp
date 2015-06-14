#include "user_defined/component/player/bombing.hh"
#include "ctvty/debug.hpp"
#include "ctvty/input.hh"
#include "ctvty/physics/raycast.hh"
#include "ctvty/component/transform.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component::player, Bombing);

namespace user_defined {
  namespace component {
    namespace player {
      Bombing::		Bombing(const serialization::Archive& __serial)
	: ctvty::MonoBehaviour<Bombing>(nullptr, "Player Bombing"), stock_max(1) {
	__serial["bombe"]	& bombe;
      }

      void	Bombing::Serialize(serialization::Archive& __serial_instance) const {
	SERIALIZE_OBJECT_AS(user_defined::component::player::Bombing, __serial_instance);
	__serial["bombe"]	& bombe;
      }

      void	Bombing::Update() {
	if (ctvty::Input::GetKeyState("put bomb " + gameObject->Name())) {
	  ctvty::debug::Log(transform->GetPosition());
	  Object::Instantiate(bombe.get(),
			      ctvty::utils::Vector3D((int)(transform->GetPosition().x + 0.5),
						     (int)(transform->GetPosition().y),
						     (int)(transform->GetPosition().z + 0.5)),
			      transform->GetRotation());
	  stock--;
	}
      }

      void	Bombing::Awake() {
	stock = 1;
	InvokeRepeating(&Bombing::Reload, 2.f, 2.f);
      }

      void	Bombing::Reload() {
	if (stock < stock_max)
	  ++stock;
      }

    };
  };
};
