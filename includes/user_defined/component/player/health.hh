#ifndef Player_Health_hh__
# define Player_Health_hh__

#include "ctvty/monobehaviour.hpp"

namespace user_defined {
  namespace component {
    namespace player {
      
      class Health : public ctvty::MonoBehaviour<Health> {
      private:
	int life;

      public:
	Health(const serialization::Archive&);
	void	Serialize(serialization::Archive&) const;

      public:
	void	TakeDamage();

      public:
	void	Heal();
      };
      
    };
  };
};

#endif
