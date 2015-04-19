#include <iostream>
#include "ctvty/assets/asset.hh"

namespace ctvty {
  namespace asset {
    Asset::		Asset(const std::string& location, std::function<void(serialization::Serializable*)> _deleter)
      : LoadedObject(), file(location), deleter(_deleter) { }

    std::shared_ptr<serialization::Serializable>
			Asset::Load() {
      std::shared_ptr<serialization::Serializable> base;

      if (!file)
	return nullptr;

      if (!LoadedObject.expired())
	return LoadedObject.lock();

      serialization::Serial*	json = serialization::Serial::InstantiateFromFile(file.GetPath());
      try {
	base.reset(serialization::Serializable::Instantiate(*json), [this](serialization::Serializable* obj) {
	    deleter(obj);
	  });
      } catch (const std::exception& e) {
	std::cout << "failed to load the file " + file.GetName() + " cause is:" << std::endl
		  << e.what() << std::endl;
	return nullptr;
      }
      LoadedObject = base;
      delete json;
      return LoadedObject.lock();
    }

    void		Asset::SetDeleter(std::function<void(serialization::Serializable*)> _deleter) {
      deleter = _deleter;
    }

    void		Asset::Save(const serialization::Serial& json) {
      std::ofstream	out(file.GetPath());

      out << json.Stringify();
    }

    filesystem::File&	Asset::GetFile() {
      return file;
    }

  };
};
