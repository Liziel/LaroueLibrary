#ifndef BroadCast_hh__
# define BroadCast_hh__

# include <list>
# include <string>

# include "ctvty/event/properties.hpp"

namespace ctvty {
  class Object;
  class GameObject;
  namespace event {
    
    class BroadCast {
    public:
      enum class Way {
	self, up, down, deletion
	  };
    private:
      GameObject*		target;
      Way			way;
      std::string		message;
      parameters::values	values;
      
    private:
      std::list<GameObject*>	removed;

    private:
      Object*			deletion;

    public:
      BroadCast(GameObject*, Way, const std::string&, parameters::values = {});
      BroadCast(const std::string&, parameters::values = {});
      
      BroadCast(GameObject*);
      BroadCast(Object*);

    public:
      ~BroadCast();

    public:
      bool		RemoveTarget(GameObject*);
      void		Dispatch();
    };
  };
};

#endif
