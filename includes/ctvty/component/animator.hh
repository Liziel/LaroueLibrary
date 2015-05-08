#ifndef Animator_hh__
# define Animator_hh__

#include "ctvty/monobehviour.hh"

namespace ctvty {
  namespace component {

    class Animator : public MonoBehviour<Animator> {
    public:
      class State : public serialization::Serializable {
	
      };
      
    private:
      std::list< std::string, std::shared_ptr<Animation> >	animations;
    };
  };
};

#endif
