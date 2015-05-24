#ifndef Asset_Texture_hh__
# define Asset_Texture_hh__

# include "serialization/serializable.hh"
# include "ctvty/rendering/texture.hh"

namespace ctvty {
  namespace asset {

    class Texture : public serialization::Serializable {
    public:
      class Wrapper : public serialization::Serializable {
      private:
	std::string					path;

      public:
	std::shared_ptr<ctvty::rendering::Texture>	model;	

      public:
			Wrapper(const serialization::Archive&);
	void		Serialize(serialization::Archive&) const;
      };
    private:
      std::string					wrapper;
      std::shared_ptr<Wrapper>				_object;

    public:
      inline
      std::shared_ptr<ctvty::rendering::Texture>&	GetShared() {
	return (_object->model);
      }
      inline
      ctvty::rendering::Texture&			GetTexture() {
	return *(_object->model);
      }

    public:
      void		Serialize(serialization::Archive&) const;
			Texture(const serialization::Archive&);

    public:
      operator bool() { return (bool) (_object && _object->model); }
      void		delayedInstantiation();
    };

  };
};

#endif
