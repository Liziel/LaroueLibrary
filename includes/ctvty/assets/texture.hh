#ifndef Asset_Texture_hh__
# define Asset_Texture_hh__

# include "serialization/serializable.hh"
# include "ctvty/rendering/texture.hh"

namespace ctvty {
  namespace asset {

    class Texture : public serialization::Serializable {
    private:
      std::string					path;
      std::shared_ptr<ctvty::rendering::Texture>	texture;

    public:
      inline
      std::shared_ptr<ctvty::rendering::Texture>&			GetTexture() {
	return texture;
      }

    public:
      void		Serialize(serialization::Archive&) const;
			Texture(const serialization::Archive&);

    public:
      void		delayedInstantiation();
    };

  };
};

#endif
