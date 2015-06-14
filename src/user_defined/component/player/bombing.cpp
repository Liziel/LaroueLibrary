#include "user_defined/component/player/bombing.hh"
#include "ctvty/debug.hpp"
#include "ctvty/input.hh"
#include "ctvty/physics/raycast.hh"
#include "ctvty/component/transform.hh"
#include "user_defined/component/bombe/core.hh"

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
	if (stock && ctvty::Input::GetKeyState("put bomb " + gameObject->Name())) {
	  ctvty::debug::Log(transform->GetPosition());
	  ctvty::GameObject* _bombe = (ctvty::GameObject*)
	    Object::Instantiate(bombe.get(),
				ctvty::utils::Vector3D((int)(transform->GetPosition().x + 0.5),
						       (int)(transform->GetPosition().y),
						       (int)(transform->GetPosition().z + 0.5)),
				transform->GetRotation());
	  bombe::Core* core = _bombe->GetComponent<bombe::Core>();
	  core->SetFuseDelay(2.f);
	  core->SetExplosionSize(1.f);
	  core->StartCountdown();
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
