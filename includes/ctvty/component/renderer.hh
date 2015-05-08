#ifndef Render_hh__
# define Render_hh__

# include "ctvty/monobehaviour.hpp"
# include "ctvty/rendering/model3d.hh"

namespace ctvty {
  namespace component {

    class Renderer : public MonoBehaviour<Renderer> {
    public:
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
      };
    private:
      std::string			file;
      std::shared_ptr<Model3D>		model;

    public:
      /*
       * archive["file"] & string --> assets
       */
			Renderer(const std::string&);
			Renderer(const serialization::Archive&);
      void		Serialize(serialization::Archive&) const override;

    public:
      ctvty::Object*	clone() const override;

    public:
      void		Awake();
      void		Render();

    public:
      void		CreateAnimation(const std::string& name, int FrameStart, int FrameEnd);
      void		PauseAnimation();
      void		SetAnimation(const std::string& name, bool loop = true);
    };


  };
};


#endif
