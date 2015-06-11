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
      if (__serial.exist("model"))
	__serial["model"] & model;
      else
	__serial["textured cube"] & texture;
      if (__serial.exist("shader"))
	__serial["shader"] & shader;
    }

    void	Renderer::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(ctvty::component::Renderer, __serial_instance);
      if (model)
	__serial["model"] & model;
      else
	__serial["textured cube"] & texture;
      if (shader)
	__serial["shader"] & shader;
      if (rotation)
	__serial["rotation"] & rotation;
    }

    void	Renderer::Awake() {
      if (shader)
	shader->delayedInstantiation();
      if (model) {
	model->delayedInstantiation();
	CreateAnimation("renderer_stop", 0, 0);
	animator = GetComponent<Animator>();
	if (animator)
	  animator->Initialize(this);
      }
      else {
	texture->delayedInstantiation();
	cube.reset(rendering::Renderer::GetRenderer().CreateCube());
      }
    }

    void	Renderer::Render()
    {
      if ((!model || !*model) && !cube)
	return ;
      if (shader && *shader)
	ctvty::rendering::Renderer::GetRenderer().UseShaderAtNextDraw(shader->GetShader());
      if (!animator && model) {
	ctvty::debug::Logs(transform->GetHierarchyPosition(), transform->GetHierarchyScale(), transform->GetHierarchyRotation());
	SetAnimation("renderer_stop");
	model->GetModel().Draw(transform->GetHierarchyPosition(),
			       transform->GetHierarchyScale(),
			       (rotation ? *rotation * transform->GetHierarchyRotation() :
				transform->GetHierarchyRotation()));
      }
      else if (model) {
	model->GetModel().Draw(transform->GetHierarchyPosition(),
			       transform->GetHierarchyScale(),
			       (rotation ? *rotation * transform->GetHierarchyRotation() :
				transform->GetHierarchyRotation()),
			       animator->GetFrame(*this));
      } else {
	texture->GetTexture().Bind();
	cube->Draw(transform->GetHierarchyPosition(),
		   transform->GetHierarchyScale(),
		   (rotation ? *rotation * transform->GetHierarchyRotation() :
		    transform->GetHierarchyRotation()));
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
