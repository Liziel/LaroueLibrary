#ifndef Bonus_hh__
# define Bonus_hh__

# include "ctvty/monobehaviour.hpp"

namespace user_defined {
  namespace component {

    class Loot : public ctvty::MonoBehaviour<Loot> {
    private:
      std::unique_ptr<ctvty::GameObject, ctvty::Object::Deleter >	loot;

    public:
      Loot(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

    public:
      void	OnDestroy();
    };

    class Bonus : public ctvty::MonoBehaviour<Bonus> {
    public:
      Bonus(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

    public:
      void	OnTriggerEnter(const ctvty::component::Collider* collider);
    };

  };
};
#endif
