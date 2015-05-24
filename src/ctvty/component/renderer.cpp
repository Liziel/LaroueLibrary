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

#include "ctvty/component/transform.hh"
#include "ctvty/component/animator.hh"

REGISTER_FOR_SERIALIZATION(ctvty::component, Renderer);

namespace ctvty
{
  namespace component
  {

    Renderer::Renderer(const serialization::Archive &__serial)
      : MonoBehaviour(nullptr, "Renderer")
    {
      if (__serial.exist("rotation"))
	__serial["rotation"] & rotation;
      __serial["model"] & model;
    }

    void	Renderer::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::component::Renderer, __serial_instance);
      __serial["model"] & model;
      if (rotation)
	__serial["rotation"] & rotation;
    }

    void	Renderer::Awake() {
      model->delayedInstantiation();
      CreateAnimation("renderer_stop", 0, 0);
      Animator* animator = GetComponent<Animator>();
      if (animator)
	animator->Initialize(this);
    }

    void	Renderer::Render()
    {
      if (!model || !*model)
	return ;
      Animator* animator = GetComponent<Animator>();
      if (!animator) {
	SetAnimation("renderer_stop");
	model->GetModel().Draw(transform->GetPosition(),
			       transform->GetScale(),
			       (rotation ? *rotation * transform->GetRotation() : transform->GetRotation()));
      }
      else {
	model->GetModel().Draw(transform->GetPosition(),
			       transform->GetScale(),
			       (rotation ? *rotation * transform->GetRotation() : transform->GetRotation()),
			       animator->GetFrame(*this));
      }
    }
    
    void	Renderer::CreateAnimation(const std::string &name, int FrameStart, int FrameEnd)
    {
      if (model && *model)
	model->GetModel().CreateAnimation(name, FrameStart, FrameEnd);
    }
    
    void	Renderer::PauseAnimation()
    {
      if (model && *model)
	model->GetModel().PauseAnimation();
    }

    void	Renderer::SetAnimation(const std::string &name, bool loop)
    {
      if (model && *model)
	model->GetModel().SetAnimation(name, loop);
    }

    float	Renderer::GetFrameDuration() {
      if ( !(model || *model) )
	return 0.f;
      return model->GetModel().GetFrameDuration();
    }
  };
};
