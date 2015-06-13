#include "user_defined/component/player/bombing.hh"
#include "ctvty/debug.hpp"

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

      }

      void	Bombing::Awake() {
	stock = 1;
	InvokeRepeating(&Bombing::Reload, 2.f, 2.f);
      }

      void	Bombing::OnTriggerEnter(const ctvty::component::Collider* co) {
	std::cout << "yep" << std::endl;
	if (co == nullptr)
	  std::cout << "wut the wat" << std::endl;
	else if (0)
	  ctvty::debug::Log(co);
      }

      void	Bombing::Reload() {
	if (stock < stock_max)
	  ++stock;
      }

    };
  };
};
