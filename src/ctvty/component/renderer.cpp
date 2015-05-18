//
// renderer.cpp for bomber in /home/tulkas/local/Projets/BomberU/src/ctvty/component
//
// Made by delesp_j
// Login   <delesp_j@epitech.net>
//
// Started on  Wed May  6 16:43:28 2015 delesp_j
// Last update Wed May  6 17:23:29 2015 delesp_j
//

#include "ctvty/application.hh"
#include "ctvty/component/renderer.hh"
#include "ctvty/rendering/renderer.hh"
#include "ctvty/component/transform.hh"
#include "ctvty/component/animator.hh"
#include "ctvty/debug.hpp"
REGISTER_FOR_SERIALIZATION(ctvty::component, Renderer);
REGISTER_FOR_SERIALIZATION(ctvty::component::Renderer, Model3D);

namespace ctvty
{
  namespace component
  {
    Renderer::Renderer(const std::string& path)
      : MonoBehaviour(nullptr, "Renderer"), file(path) {}
    Renderer::Renderer(const serialization::Archive &archive)
      : Renderer(archive["file"].as<std::string>())
    { }

    void	Renderer::Serialize(serialization::Archive& instance_archive) const {
      SERIALIZE_OBJECT_AS(ctvty::component::Renderer, instance_archive);
      __serial["file"] & file;
      if (model)
	Application::Assets().GetAsset(file).Save(model);
    }

    Object*	Renderer::clone() const {
      return new Renderer(file);
    }

    void	Renderer::Awake() {
      model = Application::Assets().GetAsset(file).LoadAs<Renderer::Model3D>();
      if (!model) {
	std::cerr << "Renderer error: no model definition at " << file << std::endl;
	return ;
      }
      CreateAnimation("renderer_stop", 0, 0);
      for (Animator* animator : GetComponents<Animator>())
	animator->Initialize(this);
    }

    void	Renderer::Render()
    {
      if (!model)
	return ;

      Animator* animator = GetComponent<Animator>();
      if (!animator) {
	SetAnimation("renderer_stop");
	model->GetModel().Draw(transform->GetPosition(),
			       transform->GetScale(),
			       transform->GetRotation());
      }
      else {
	model->GetModel().Draw(transform->GetPosition(),
			       transform->GetScale(),
			       transform->GetRotation(),
			       animator->GetFrame(*this));
      }
    }
    
    void	Renderer::CreateAnimation(const std::string &name, int FrameStart, int FrameEnd)
    {
      if (model)
	model->GetModel().CreateAnimation(name, FrameStart, FrameEnd);
    }
    
    void	Renderer::PauseAnimation()
    {
      if (model)
	model->GetModel().PauseAnimation();
    }

    void	Renderer::SetAnimation(const std::string &name, bool loop)
    {
      if (model)
	model->GetModel().SetAnimation(name, loop);
    }

    float	Renderer::GetFrameDuration() {
      if (model)
	return model->GetModel().GetFrameDuration();
      return 0.f;
    }



    Renderer::Model3D::		Model3D(const serialization::Archive& archive) {
      archive["path"] & path;
      model.reset(ctvty::rendering::Renderer::GetRenderer().Load3DModel(path));
    }

    void			Renderer::Model3D::Serialize(serialization::Archive& archive_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::component::Renderer::Model3D, archive_instance)
      __serial["path"] & path;
    }
    
  };
};
