#ifndef Bombe_Core_hh__
# define Bombe_Core_hh__

#include "ctvty/monobehaviour.hpp"

namespace user_defined {
  namespace component {
    namespace bombe {
      
      class Core : public ctvty::MonoBehaviour<Core> {
      private:
	std::unique_ptr<ctvty::GameObject>	damage_zone;

      public:
	Core(const serialization::Archive&);
	void	Serialize(serialization::Archive&) const;

      private:
	void		Explode();

      public:
	void		OnTriggerExit(const ctvty::component::Collider*);

      private:
	float		fuseDelay;
	float		size;
      public:
	inline void	SetFuseDelay(float d) { fuseDelay = d; }
	inline void	SetExplosionSize(float s) { size = s; }

      public:
	void		StartCountdown();
      };

    };
  };
};

#endif
