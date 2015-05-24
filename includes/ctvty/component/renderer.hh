#ifndef Render_hh__
# define Render_hh__

# include "ctvty/monobehaviour.hpp"
# include "ctvty/assets/model3d.hh"

namespace ctvty {
  namespace component {

    class Renderer : public MonoBehaviour<Renderer> {
    private:
      std::unique_ptr<asset::Model3D>		model;
      std::unique_ptr<utils::Quaternion>	rotation;

    public:
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
      float		GetFrameDuration();
    };


  };
};


#endif
