#ifndef Asset_hh__
# define Asset_hh__

# include <memory>

# include "filesystem/file.hh"
# include "serialization/serializable.hh"

namespace ctvty {
  namespace asset {
    class Asset {
    private:
      std::weak_ptr<serialization::Serializable>	LoadedObject;
      filesystem::File					file;

    public:
      Asset(const std::string&);

    public:
      template<typename _type>
      std::shared_ptr<_type>				LoadAs() { return std::dynamic_pointer_cast<_type>(Load()); }
      std::shared_ptr<serialization::Serializable>	Load();

    public:
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
