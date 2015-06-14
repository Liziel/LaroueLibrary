#include "user_defined/component/bombe/core.hh"
#include "ctvty/component/collider.hh"
#include "ctvty/component/transform.hh"
#include "ctvty/physics/raycast.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component::bombe, Core);

namespace user_defined {
  namespace component {
    namespace bombe {

      Core::Core(const serialization::Archive& __serial)
	: ctvty::MonoBehaviour<Core>(nullptr, "Bombe Core") {
	__serial["damage zone"] & damage_zone;
	exploded = false;
	RegisterListener("Explosion", &Core::BeforeExplode);
      }

      void	Core::Serialize(serialization::Archive& __serial_instance) const {
	SERIALIZE_OBJECT_AS(user_defined::component::bombe::Core, __serial_instance);
	__serial["damage zone"] & damage_zone;
      }

      void	Core::StartCountdown() {
	Invoke(&Core::Explode, fuseDelay);
      }
      
      void	Core::OnTriggerExit(const ctvty::component::Collider*) {
	GetComponent<ctvty::component::Collider>()->trigger() = false;
      }

      void	Core::BeforeExplode() {
	if (exploded)
	  return ;
	CancelInvoke();
	Explode();
      }

      void	Core::Explode() {
	{
	exploded = true;
	  ctvstd::Optional<ctvty::physics::Raycaster::Hit>	hit =
	    ctvty::physics::Raycaster::Raycast(transform->GetPosition() + ctvty::utils::Vector3D::forward * 0.5,
					       ctvty::utils::Vector3D::forward,
					       size,
					       [this] (ctvty::component::Collider* co) {
						 return co->GetGameObject() != gameObject &&
						 !co->GetGameObject()->CompareTag("player") &&
						 !co->GetGameObject()->CompareTag("zone") &&
						 !co->GetGameObject()->CompareTag("ia");
					       });
	  if (hit) {
	    if (hit->collider->GetGameObject()->CompareTag("bombes")) {
	      std::cout << "wut" << std::endl;
	      hit->collider->BroadcastMessage("Explosion");
	    }
	    else if (hit->collider->GetGameObject()->Name() == "destructible")
	      Object::Destroy(hit->collider->GetGameObject());
	  }
	  for (float i = 0; i < (hit ? (int)hit->dist : size); ++i)
	    Object::Destroy(Object::Instantiate(damage_zone.get(),
						transform->GetPosition()
						+ ctvty::utils::Vector3D::forward * (i + 1),
						ctvty::utils::Quaternion::identity), 0.5);
	}
	{
	  ctvstd::Optional<ctvty::physics::Raycaster::Hit>	hit =
	    ctvty::physics::Raycaster::Raycast(transform->GetPosition() + ctvty::utils::Vector3D::left * 0.5,
					       ctvty::utils::Vector3D::left,
					       size,
					       [this] (ctvty::component::Collider* co) {
						 return co->GetGameObject() != gameObject &&
						 !co->GetGameObject()->CompareTag("player") &&
						 !co->GetGameObject()->CompareTag("zone") &&
						 !co->GetGameObject()->CompareTag("ia");
					       });
	  if (hit) {
	    if (hit->collider->GetGameObject()->CompareTag("bombes")) {
	std::cout << "wut" << std::endl;
	      hit->collider->BroadcastMessage("Explosion");
	    }
	    else if (hit->collider->GetGameObject()->Name() == "destructible")
	      Object::Destroy(hit->collider->GetGameObject());
	  }
	  for (float i = 0; i < (hit ? (int)hit->dist : size); ++i)
	    Object::Destroy(Object::Instantiate(damage_zone.get(),
						transform->GetPosition()
						+ ctvty::utils::Vector3D::left * (i + 1),
						ctvty::utils::Quaternion::identity), 0.5);
	}
	{
	  ctvstd::Optional<ctvty::physics::Raycaster::Hit>	hit =
	    ctvty::physics::Raycaster::Raycast(transform->GetPosition() + ctvty::utils::Vector3D::right * 0.5,
					       ctvty::utils::Vector3D::right,
					       size,
					       [this] (ctvty::component::Collider* co) {
						 return co->GetGameObject() != gameObject &&
						 !co->GetGameObject()->CompareTag("zone") &&
						 !co->GetGameObject()->CompareTag("player") &&
						 !co->GetGameObject()->CompareTag("ia");
					       });
	  if (hit) {
	    if (hit->collider->GetGameObject()->CompareTag("bombes")) {
	std::cout << "wut" << std::endl;
	      hit->collider->BroadcastMessage("Explosion");
	    }
	    else if (hit->collider->GetGameObject()->Name() == "destructible")
	      Object::Destroy(hit->collider->GetGameObject());
	  }
	  for (float i = 0; i < (hit ? (int)hit->dist : size); ++i)
	    Object::Destroy(Object::Instantiate(damage_zone.get(),
						transform->GetPosition()
						+ ctvty::utils::Vector3D::right * (i + 1),
						ctvty::utils::Quaternion::identity), 0.5);
	}
	{
	  ctvstd::Optional<ctvty::physics::Raycaster::Hit>	hit =
	    ctvty::physics::Raycaster::Raycast(transform->GetPosition() + ctvty::utils::Vector3D::back * 0.5,
					       ctvty::utils::Vector3D::back,
					       size,
					       [this] (ctvty::component::Collider* co) {
						 return co->GetGameObject() != gameObject &&
						 !co->GetGameObject()->CompareTag("player") &&
						 !co->GetGameObject()->CompareTag("zone") &&
						 !co->GetGameObject()->CompareTag("ia");
					       });
	  if (hit) {
	    if (hit->collider->GetGameObject()->CompareTag("bombes")) {
	std::cout << "wut" << std::endl;
	      hit->collider->BroadcastMessage("Explosion");
	    }
	    else if (hit->collider->GetGameObject()->Name() == "destructible")
	      Object::Destroy(hit->collider->GetGameObject());
	  }
	  for (float i = 0; i < (hit ? (int)hit->dist : size); ++i)
	    Object::Destroy(Object::Instantiate(damage_zone.get(),
						transform->GetPosition()
					      + ctvty::utils::Vector3D::back * (i + 1),
						ctvty::utils::Quaternion::identity), 0.5);
	}
	Object::Destroy(gameObject, 0.5);
      }
    };
  };
};
