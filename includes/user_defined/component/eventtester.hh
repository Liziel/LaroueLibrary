#ifndef EventTester_hh__
# define EventTester_hh__

# include "ctvty/monobehaviour.hpp"

# include "ctvty/utils/collision.hh"

namespace user_defined {
  namespace component {
    
    class EventTester : public ctvty::MonoBehaviour<EventTester> {
    public:
			EventTester();

    public:
			EventTester(const serialization::Archive&);
      void		Serialize(serialization::Archive&) const override;

    public:
      void		OnCollisionEnter(const ctvty::utils::Collision*);
      void		OnCollisionStay(const ctvty::utils::Collision*);
      void		OnCollisionExit(const ctvty::utils::Collision*);

    public:
      ctvty::Object*	clone() const override;
    };
  };
};


#endif
