#include "user_defined/component/player/health.hh"
#include "ctvty/component/hud.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component::player, Health);

namespace user_defined {
  namespace component {
    namespace player {
      
      Health::		Health(const serialization::Archive& __serial)
	: ctvty::MonoBehaviour<Health>(nullptr, "Player Health") {
	RegisterListener("Damage", &Health::TakeDamage);
      }

      void	Health::Serialize(serialization::Archive& __serial_instance) const {
	SERIALIZE_OBJECT_AS(user_defined::component::player::Health, __serial_instance);
      }

      void	Health::TakeDamage() {
	if (life > 1) {
	  ctvty::component::Canvas* c = GetComponent<ctvty::component::Canvas>();
	  if (c) {
	    (*c)[std::string("life ") + static_cast<char>(life + '0')]->disable();
	  }
	  --life;
	}
	if (life == 1) {
	  BroadcastAll(gameObject->Name() + " death");
	  Object::Destroy(gameObject);
	}
      }

      void	Health::Heal() {
	if (life < 3) {
	  ctvty::component::Canvas* c = GetComponent<ctvty::component::Canvas>();
	  if (c) {
	    (*c)[std::string("life ") + static_cast<char>(life + '0')]->enable();
	  }
	  ++life;
	}
      }

    };
  };
};
