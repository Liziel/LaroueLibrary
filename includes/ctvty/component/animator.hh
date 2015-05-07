#ifndef Animator_hh__
# define Animator_hh__

#include "ctvty/monobehviour.hh"

namespace ctvty {
  namespace component {

    class Animator : public MonoBehviour<Animator> {
    public:
      class Animation : public serialization::Serializable { };
      class Trigger : public serialization::Serializable { };
      
    private:
      std::list< std::shared_ptr<Animation> >	animations;
      std::list< std::shared_ptr<Trigger> >	animations;
    };
  };
};

#endif
