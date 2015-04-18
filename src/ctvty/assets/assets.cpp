#include "ctvty/assets/assets.hh"

namespace ctvty {
  namespace asset {
    
    Assets::	Assets(const std::string& location)
      : directory(location) {}
    Assets::	Assets(const filesystem::Directory& location)
      : directory(location) {}

    Assets	Assets::GetAssets(const std::string& _relative_path) {
      filesystem::Directory	_directory(directory.GetPath() + '/' + _relative_path);

      if (!_directory)
	_directory.Create();
      return (_directory);
    }

    Asset&	Assets::GetAsset(const std::string& _relative_path) {
      filesystem::File	file(directory.GetPath() + '/' + _relative_path);

      if (!file)
	file.Create();
      if (Assets::GetCachedAssets().find(file.GetPath()) == Assets::GetCachedAssets().end())
	  Assets::GetCachedAssets().emplace(file.GetPath(), file.GetPath());
      return Assets::GetCachedAssets().at(file.GetPath());
    }

    std::map<std::string, Asset>&
		Assets::GetCachedAssets() {
      static std::map<std::string, Asset> _map;
      return _map;
    }

  };
};
