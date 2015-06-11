#ifndef Render_hh__
# define Render_hh__

# include "ctvty/monobehaviour.hpp"
# include "ctvty/assets/texture.hh"
# include "ctvty/assets/model3d.hh"
# include "ctvty/assets/shader.hh"

namespace ctvty {
  namespace component {

    class Renderer : public MonoBehaviour<Renderer> {
    private:
      std::unique_ptr<asset::Shader>		shader;
      std::unique_ptr<asset::Model3D>		model;
      std::unique_ptr<asset::Texture>		texture;
      std::unique_ptr<utils::Quaternion>	rotation;
      std::unique_ptr<rendering::Model3D>	cube;

    public:
			Renderer(const serialization::Archive&);
      void		Serialize(serialization::Archive&) const override;

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
