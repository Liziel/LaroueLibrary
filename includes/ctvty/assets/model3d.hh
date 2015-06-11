#ifndef Asset_Model3D_hh__
# define Asset_Model3D_hh__

# include "serialization/serializable.hh"
# include "ctvty/rendering/model3d.hh"

namespace ctvty {
  namespace asset {

    class Model3D : public serialization::Serializable {
    public:
      class Wrapper : public serialization::Serializable {
      private:
	std::string					path;

      public:
	std::shared_ptr<ctvty::rendering::Model3D>	model;	

      public:
			Wrapper(const serialization::Archive&);
	void		Serialize(serialization::Archive&) const;
      };

    private:
      std::string					wrapper;
      std::shared_ptr<Wrapper>				_object;

    public:
      inline
      std::shared_ptr<ctvty::rendering::Model3D>&	GetShared() {
	return (_object->model);
      }
      inline
      ctvty::rendering::Model3D&			GetModel() {
	return *(_object->model);
      }

    public:
      Model3D(const serialization::Archive&);
      void		Serialize(serialization::Archive&) const;

    public:
      inline
      operator bool() { return (bool) (_object && _object->model); }
      void		delayedInstantiation();
    };

  };
};


#endif
