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

REGISTER_FOR_SERIALIZATION(ctvty::component, Renderer);
REGISTER_FOR_SERIALIZATION(ctvty::component::Renderer, Model3D);

namespace ctvty
{
  namespace component
  {
    Renderer::Renderer(const serialization::Archive &archive)
    {
      archive["file"] & file;
      archive["model"] & model;
      model = Application::Assets().GetAsset(file).LoadAs<Model3D>();
    }

    void	Renderer::Serialize(serialization::Archive& instance_archive) const
    {
      SERIALIZE_OBJECT_AS(ctvty::component::Renderer, instance_archive);
      __serial["file"] & file;
      Application::Assets().GetAsset(file).Save(model);
    }
    
    void	Renderer::Render()
    {
      model->GetModel().Draw(transform->GetPosition(),
			     transform->GetScale(),
			     transform->GetRotation());
    }
    
    void	Renderer::CreateAnimation(const std::string &name, int FrameStart, int FrameEnd)
    {
      model->GetModel().CreateAnimation(name, FrameStart, FrameEnd);
    }
    
    void	Renderer::PauseAnimation()
    {
      model->GetModel().PauseAnimation();
    }

    void	Renderer::SetAnimation(const std::string &name, bool loop)
    {
      model->GetModel().SetAnimation(name, loop);
    }

    Renderer::Model3D::Model3D(const serialization::Archive& archive) {
      archive["path"] & path;
      model.reset(ctvty::renderer::Renderer::GetRenderer().Load3DModel(s));
    }

    void		Serialize(serialization::Archive& archive_instance) {
      SERIALIZE_OBJECT_AS(ctvty::component::Renderer::Model3D, archive_instance)
      __serial["path"] & path;
    }
    
  };
};
