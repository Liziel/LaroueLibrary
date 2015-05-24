#ifndef Asset_Texture_hh__
# define Asset_Texture_hh__

# include "serialization/serializable.hh"
# include "ctvty/rendering/model3d.hh"

namespace ctvty {
  namespace asset {

    class Model3D : public serialization::Serializable {
    private:
      std::string					path;
      std::shared_ptr<ctvty::rendering::Model3D>	model;
    public:
      inline
      ctvty::rendering::Model3D&			GetModel() {
	return *model;
      }

    public:
      Model3D(const serialization::Archive&);
      void		Serialize(serialization::Archive&) const;

    public:
      operator bool() { return (bool) model; }
      void		delayedInstantiation();
    };

  };
};


#endif
