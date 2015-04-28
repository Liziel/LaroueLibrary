#ifndef Asset_hh__
# define Asset_hh__

# include <memory>
# include <functional>

# include "filesystem/file.hh"
# include "serialization/serializable.hh"

namespace ctvty {
  namespace asset {
    class Asset {
    private:
      std::weak_ptr<serialization::Serializable>	LoadedObject;
      filesystem::File					file;
      std::function<void(serialization::Serializable*)>	deleter;

    public:
      Asset(const std::string&,
	    std::function<void(serialization::Serializable*)> = [] (serialization::Serializable* obj) {
	      delete obj;
	    });
    
    public:
      void						SetDeleter(std::function<void(serialization::Serializable*)>);

    public:
      /*
       * Load :
		Each time we load, always the same instance of the loaded object
       * LoadAs :
		same as load but dynamic cast to wanted type
       */
      template<typename _type>
      std::shared_ptr<_type>				LoadAs() { return std::dynamic_pointer_cast<_type>(Load()); }
      std::shared_ptr<serialization::Serializable>	Load();

    public:
      /*
       * Inverse of Load, will save the given serializable object in the json file
       *  -> Integer, Floating, String, Container< Serializable >, std::shared_ptr< Serializable >, Object inheriting serialization::Serializable
       */
      void						Save(const serialization::Serial&);
      template<typename _type>
      void						Save(const _type& to_serialize) {
	serialization::Serial	json; json & to_serialize;
	Save(json);
      }

    public:
      filesystem::File&					GetFile();
    };
  };
};

#endif
