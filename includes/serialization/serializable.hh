#ifndef Serialization_hh__
# define Serialization_hh__

# include <map>
# include <string>
# include <functional>
# include <typeinfo>
# include <regex>

#include "serialization/archive.hh"

namespace serialization {
  class Serializable {
  private:
    static std::map< std::string, std::function<Serializable*(const Archive&)> >&	getAllocationMap();
    static std::map< std::string, std::string>&						getTypeIdMap();

  public:
    template<typename _type>
    static void				CreateSerializableInvoker(const std::string& name) {
      getAllocationMap()
	.emplace(name,
		 [] (const Archive& a) ->Serializable* { return new _type (a); } );
      getTypeIdMap()
	.emplace(name,
		 typeid(_type).name());
    }

    static Serializable*		Instantiate(Archive& archive);

  public:
    virtual void			Serialize(Archive&) = 0;
  };

  template<typename _type>
  struct Registration {
    static int	_register;
  };

  template<typename _type>
  int	Registration<_type>::_register([] () -> int {
      std::string		name(__PRETTY_FUNCTION__);
      std::regex		regex(".* \\[.* = (.*)\\]");
      std::smatch		sm;

      std::regex_match	(name, sm, regex); name = sm[1];

      Serializable::CreateSerializableInvoker<_type>(name);
      return (0);
    }());
};

# define REGISTER_FOR_SERIALIZATION(__type__)				\
  void	 register_for_serialization ## __type__ () {			\
    Registration< __type__ >::_register;				\
  }

#endif
