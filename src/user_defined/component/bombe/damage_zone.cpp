#include "user_defined/component/bombe/damage_zone.hh"
#include "ctvty/debug.hpp"

REGISTER_FOR_SERIALIZATION(user_defined::component::bombe, DamageZone);

namespace user_defined {
  namespace component {
    namespace bombe {

      DamageZone::DamageZone(const serialization::Archive& __serial)
	: ctvty::MonoBehaviour<DamageZone>(nullptr, "Bombe DamageZone") {
      }

      void	DamageZone::Serialize(serialization::Archive& __serial_instance) const {
	SERIALIZE_OBJECT_AS(user_defined::component::bombe::DamageZone, __serial_instance);
      }

      void	DamageZone::OnTriggerEnter(const ctvty::component::Collider* collider) {
	collider->BroadcastMessage("Damage");
      }
    };
  };
};
