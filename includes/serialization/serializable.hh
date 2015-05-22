#ifndef Serialization_hh__
# define Serialization_hh__

# include <map>
# include <string>
# include <functional>
# include <typeinfo>

#include "serialization/archive.hh"

namespace serialization {
  class Serializable {
  private:
    static std::map< std::string, std::function<Serializable*(const Archive&)> >&	getAllocationMap();

  public:
    static std::map< std::string, std::string >&					getTypeIdMap();

  public:
    template<typename _type>
    static void				CreateSerializableInvoker(const std::string& name) {
      getAllocationMap()
	.emplace(name,
		 [] (const Archive& a) ->Serializable* { return new _type (a); } );
      getTypeIdMap()
	.emplace(typeid(_type).name(),
		 name);
    }

    static Serializable*		Instantiate(const Archive& archive);
    static Serializable*		Instantiate(const Serial& archive);

  public:
    virtual void			Serialize(Archive&) const = 0;

  public:
    virtual ~Serializable() {}
  };

  template<typename _type>
  struct Registration {
    static int	_register;
  };

  template<typename _type>
  int	Registration<_type>::_register([] () -> int {
      std::string		name(__PRETTY_FUNCTION__);
      name = name.substr(name.find('['));
      name = name.substr(name.find('='));
      name = name.substr(2, name.find(']') - 2);
      Serializable::CreateSerializableInvoker<_type>(name);
      return (0);
    }());
};

# define REGISTER_FOR_SERIALIZATION(__namespace__, __type__)		\
  void	 register_for_serialization_ ## __type__ () {			\
    (void)serialization::Registration< __namespace__ :: __type__ >::_register; \
  }

# define SERIALIZE_OBJECT_AS(__type__, __serial__)			\
  serialization::Archive* __new_serial =				\
    new serialization::serial::object;					\
  serialization::Archive& __serial = *__new_serial;			\
  __serial__.emplace("object",						\
		     new serialization::Serial( __new_serial ));	\
  __serial__["type"] & std::string( #__type__ );			\


# define EASILY_SERIALIZABLE(__class__, __initializer_list__,  __serialization__) \
  void	Serialize(serialization::Archive& __serial_instance) {		\
    SERIALIZE_OBJECT_AS( __class__, __serial_instance);			\
    __serialization__;							\
  };									\
  __class__(const serialization::Archive& __serial)			\
  __initializer_list__ {						\
    __serialization__;							\
  };									\

#endif
