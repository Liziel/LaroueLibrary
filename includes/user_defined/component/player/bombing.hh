#ifndef _Bombing_hh__
# define _Bombing_hh__

# include "ctvty/monobehaviour.hpp"
# include "ctvty/component/collider.hh"

namespace user_defined {
  namespace component {
    namespace player {

      class Bombing : public ctvty::MonoBehaviour<Bombing> {
      private:
	std::shared_ptr<ctvty::GameObject>	bombe;
	float					stock, stock_max;

      public:
	Bombing(const serialization::Archive& __serial);
	void	Serialize(serialization::Archive&) const;

      public:
	void	Update();

      public:
	void	Awake();
	void	Reload();
      };

    };
  };
};

#endif
