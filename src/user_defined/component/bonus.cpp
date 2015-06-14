#include <cstdlib>
#include <random>

#include "user_defined/component/bonus.hh"
#include "ctvty/component/transform.hh"
#include "ctvty/component/collider.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, Loot);
REGISTER_FOR_SERIALIZATION(user_defined::component, Bonus);

namespace user_defined {
  namespace component {

    Loot::Loot(const serialization::Archive& __serial)
      : ctvty::MonoBehaviour<Loot>(nullptr, "Loot") {
      __serial["loot"] & loot;
    }

    void	Loot::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::Loot, __serial_instance);
      __serial["loot"] & loot;
    }

    void	Loot::OnDestroy() {
      if (!(std::rand() % 3)) {
	Object::Instantiate(loot.get(), 
			    transform->GetPosition(),
			    ctvty::utils::Quaternion::identity);
      }
    }

    Bonus::Bonus(const serialization::Archive& __serial)
      : ctvty::MonoBehaviour<Bonus>(nullptr, "Bonus") {
    }

    void	Bonus::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::Bonus, __serial_instance);
    }

    void	Bonus::OnTriggerEnter(const ctvty::component::Collider* collider) {
      if (const_cast<ctvty::component::Collider*>(collider)->GetGameObject()->CompareTag("player")) {
	Object::Destroy(gameObject);
	switch (std::rand() % 3) {
	case 0:
	  collider->BroadcastMessage("grenade token");
	  break;
	case 1:
	  collider->BroadcastMessage("stock up");
	  break;
	case 2:
	  collider->BroadcastMessage("size up");
	  break;
	}
      }
    }
  };
};
