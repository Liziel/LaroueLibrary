#ifndef Cache_hh__
# define Cache_hh__

# include <list>
# include <functional>
# include <memory>

# include "ctvty/assets/asset.hh"

namespace ctvty {
  namespace asset {
    template<typename type = serialization::Serializable>
    class Cache {
    private:
      std::function<bool (Asset&)>			_accept;
      std::list< std::shared_ptr<type> >		_cached;

    public:
      Cache(std::function<bool (Asset&)> __accept
	    = [] (Asset& file) -> bool {
	      return file.GetFile().GetName().substr(-sizeof("json") + 1, sizeof("json")) == "json";
	    }) : _accept(__accept) {}
      ~Cache() {}

    public:
      bool		Accept(Asset& file) { return _accept(file); }
      void		Store(std::shared_ptr<type> to_cache) {
	if (to_cache)
	  _cached.emplace_back(to_cache);
      }
    };
  };
};
#endif
