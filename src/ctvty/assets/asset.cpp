#include "ctvty/assets/asset.hh"

namespace ctvty {
  namespace asset {
    Asset::		Asset(const std::string& location)
      : LoadedObject(), file(location) { }

    std::shared_ptr<serialization::Serializable>
			Asset::Load() {
      std::shared_ptr<serialization::Serializable> base;

      if (!file)
	return nullptr;

      if (!LoadedObject.expired())
	return LoadedObject.lock();

      serialization::Serial*	json = serialization::Serial::InstantiateFromFile(file.GetPath());
      base.reset(serialization::Serializable::Instantiate(*json));
      LoadedObject = base;
      delete json;
      return LoadedObject.lock();
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
