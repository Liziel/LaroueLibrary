#ifndef Assets_hh__
# define Assets_hh__

# include <map>

# include "filesystem/directory.hh"

# include "ctvty/assets/asset.hh"
# include "ctvty/assets/cache.hh"

namespace ctvty {
  namespace asset {
    /*
     * The Assets class represent an asset directory
     */
    class Assets {
    public:
      Assets(const std::string& location);
      Assets(const filesystem::Directory& location);
      Assets(Assets&&) = default;

    public:
      template <typename type>
      void	Cache(Cache<type>* cache) {
	if (!directory)
	  return ;
	for (const filesystem::File& file : directory)
	  if (file.IsDirectory() && file.GetName() != "." && file.GetName() != "..")
	    Assets(file.GetPath()).Cache<type>(cache);
	  else {
	    if (Assets::GetCachedAssets().find(file.GetPath()) == Assets::GetCachedAssets().end())
	      Assets::GetCachedAssets().emplace(file.GetPath(), file.GetPath());
	    ctvty::asset::Asset& asset = Assets::GetCachedAssets().at(file.GetPath());
	    if (cache->Accept(asset))
	      cache->Store(asset.LoadAs<type>());
	  }
      }

      Assets				GetAssets(const std::string&);
      Asset&				GetAsset(const std::string&);

    private:
      filesystem::Directory		directory;

    public:
      static std::map<std::string, Asset>&		GetCachedAssets();
    };
  };
};

#endif
