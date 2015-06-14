#ifndef Menu_In_Game_Instantiation_hh__
# define Menu_In_Game_Instantiation_hh__

# include "ctvty/monobehaviour.hpp"

namespace user_defined {
  namespace component {
    
    class MenuInGameInstantiation : public ctvty::MonoBehaviour<MenuInGameInstantiation> {
    private:
      std::unique_ptr<ctvty::GameObject, ctvty::Object::Deleter>	MenuInGame;
      ctvty::GameObject*			instantied;
      bool					loaded;

    public:
      MenuInGameInstantiation(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

    public:
      void	Update();
      void	Resume();
    };

  };
};

#endif
