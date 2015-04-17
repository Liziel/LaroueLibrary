#ifndef Assets_hh__
# define Assets_hh__

# include <map>

# include "filesystem/directory.hh"

# include "ctvty/assets/asset.hh"

namespace ctvty {
  namespace asset {
    /*
     * The Assets class represent an asset directory
     */
    class Assets {
    public:
      Assets(const std::string& location);

    public:
      void				CacheTree();
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
