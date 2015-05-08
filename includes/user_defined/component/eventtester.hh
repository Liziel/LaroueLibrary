#ifndef EventTester_hh__
# define EventTester_hh__

# include "ctvty/monobehaviour.hpp"

# include "ctvty/utils/collision.hh"

# include "ctvty/rendering/renderer.hh"

namespace user_defined {
  namespace component {
    
    class EventTester : public ctvty::MonoBehaviour<EventTester> {
    private:
      ctvty::rendering::Model3D*	model3D;

    public:
			EventTester();

    public:
			EventTester(const serialization::Archive&);
      void		Serialize(serialization::Archive&) const override;

    public:
      void		Awake();
      void		Update();

    public:
      void		OnCollisionEnter(const ctvty::utils::Collision*);
      void		OnCollisionStay(const ctvty::utils::Collision*);
      void		OnCollisionExit(const ctvty::utils::Collision*);

    public:
      void		Render();

    public:
      ctvty::Object*	clone() const override;
    };

  };
};


#endif
