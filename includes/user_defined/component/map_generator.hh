#ifndef MapGenerator_hh__
# define MapGenerator_hh__

# include "ctvty/monobehaviour.hpp"
# include "ctvty/assets/model3d.hh"
# include "user_defined/component/configurations.hh"

namespace user_defined {
  namespace component {
    
    class MapGenerator : public ctvty::MonoBehaviour<MapGenerator> {
    private:
      std::unique_ptr<ctvty::GameObject, ctvty::Object::Deleter>		ground;
      std::unique_ptr<ctvty::GameObject, ctvty::Object::Deleter>		destructible;
      std::unique_ptr<ctvty::GameObject, ctvty::Object::Deleter>		indestructible;

      std::size_t								size_x;
      std::size_t								size_y;

      std::unique_ptr<ctvty::GameObject, ctvty::Object::Deleter>		player1;
      std::unique_ptr<ctvty::GameObject, ctvty::Object::Deleter>		player2;

      std::list< std::shared_ptr<ctvty::asset::Model3D> >	bombes;

      bool					directly_generated;

    public:
		MapGenerator(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

    public:
      void	Awake();
      void	Generate(int, int);

    public:
      inline std::size_t&	SizeX() { return size_x; }
      inline std::size_t&	SizeY() { return size_y; }
    };
  };
};

#endif
