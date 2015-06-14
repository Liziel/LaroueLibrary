#ifndef Bombe_Health_hh__
# define Bombe_Health_hh__

#include "ctvty/monobehaviour.hpp"
#include "ctvty/component/collider.hh"

namespace user_defined {
  namespace component {
    namespace bombe {
      
      class DamageZone : public ctvty::MonoBehaviour<DamageZone> {
      public:
	DamageZone(const serialization::Archive&);
	void	Serialize(serialization::Archive&) const;

      public:
	void	OnTriggerEnter(const ctvty::component::Collider*);
      };
      
    };
  };
};

#endif
