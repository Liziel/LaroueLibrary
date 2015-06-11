#ifndef MapGenerator_hh__
# define MapGenerator_hh__

# include "ctvty/monobehaviour.hpp"

namespace user_defined {
  namespace component {
    
    class MapGenerator : public ctvty::MonoBehaviour<MapGenerator> {
    private:
      std::unique_ptr<ctvty::GameObject>	ground;
      std::unique_ptr<ctvty::GameObject>	destructible;
      std::unique_ptr<ctvty::GameObject>	indestructible;

      std::size_t				size_x;
      std::size_t				size_y;

      std::unique_ptr<ctvty::GameObject>	player1;
      std::unique_ptr<ctvty::GameObject>	player2;

      bool					directly_generated;

    public:
		MapGenerator(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

    public:
      void	Awake();
      void	OnLoadScene(const std::string& scene_name);

    public:
      inline std::size_t&	SizeX() { return size_x; }
      inline std::size_t&	SizeY() { return size_y; }
    };
  };
};

#endif
