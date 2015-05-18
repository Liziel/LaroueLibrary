#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gdlimpl/renderer.hh"
#include "gdlimpl/model3d.hh"

namespace GdlImpl {
  Model3D::		Model3D(Renderer& r, const std::string& file)
    : path(file), model(new gdl::Model), renderer(r) {
    model->load(file);
  }

  void			Model3D::CreateAnimation(const std::string& name,
						 int frameStart, int frameEnd) {
    model->createSubAnim(0, name, frameStart, frameEnd);
  }

  void			Model3D::PauseAnimation() {
    model->pause(false);
  }

  void			Model3D::SetAnimation(const std::string& name, bool loop) {
    model->setCurrentSubAnim(name, loop);
  }

  float			Model3D::GetFrameDuration() {
    return model->getFrameDuration();
  }

  void			Model3D::Draw(const ctvty::utils::Vector3D& position,
				      const ctvty::utils::Vector3D& scale,
				      const ctvty::utils::Quaternion& rotation,
				      double time) {
    glm::mat4 transform(1);
    ctvty::utils::Vector3D	r = rotation.Complex();

    transform = glm::translate(transform, glm::vec3(position.x, position.y, -position.z));
    transform = glm::rotate(transform, r.x, glm::vec3(1., 0., 0.));
    transform = glm::rotate(transform, r.y, glm::vec3(0., 1., 0.));
    transform = glm::rotate(transform, r.z, glm::vec3(0., 0., 1.));
    transform = glm::scale(transform, glm::vec3(scale.x, scale.y, scale.z));
    model->draw(renderer.GetShader(), transform, time);
  }
};
