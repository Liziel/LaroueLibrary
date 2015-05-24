#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "gdlimpl/renderer.hh"
#include "gdlimpl/model3d.hh"

namespace GdlImpl {
  Model3D::		Model3D(Renderer& r, const std::string& file)
    : path(file), model(new gdl::Model), renderer(r) {
    loadStatus = true;
    if (!model->load(file))
      loadStatus = false;
  }

  void			Model3D::CreateAnimation(const std::string& name,
						 int frameStart, int frameEnd) {
    if (!loadStatus)
      return ;
    model->createSubAnim(0, name, frameStart, frameEnd);
  }

  void			Model3D::PauseAnimation() {
    if (!loadStatus)
      return ;
    model->pause(false);
  }

  void			Model3D::SetAnimation(const std::string& name, bool loop) {
    if (!loadStatus)
      return ;
    model->setCurrentSubAnim(name, loop);
  }

  float			Model3D::GetFrameDuration() {
    if (!loadStatus)
      return 0.f;
    return model->getFrameDuration();
  }

  void			Model3D::Draw(const ctvty::utils::Vector3D& position,
				      const ctvty::utils::Vector3D& scale,
				      const ctvty::utils::Quaternion& rotation,
				      double time) {
    if (!loadStatus)
      return ;
    ctvty::utils::Vector3D	r = rotation.Complex();
    glm::mat4 transform; 
    transform = glm::translate(position.x, position.y, position.z);
    transform *= glm::scale(scale.x, scale.y, scale.z);
    transform *= glm::rotate((r.x / glm::pi<float>()) * 180.f, glm::vec3(1., 0., 0.));
    transform *= glm::rotate((r.y / glm::pi<float>()) * 180.f, glm::vec3(0., 1., 0.));
    transform *= glm::rotate((r.z / glm::pi<float>()) * 180.f, glm::vec3(0., 0., 1.));
    model->draw(renderer.GetShader(), transform, time);
  }
};
