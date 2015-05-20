#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

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
    ctvty::utils::Vector3D	r = rotation.Complex();
    glm::mat4 transform = glm::scale(scale.x, scale.y, scale.z);
    transform *= glm::rotate((r.x / glm::pi<float>()) * 180.f, glm::vec3(1., 0., 0.));
    transform *= glm::rotate((r.y / glm::pi<float>()) * 180.f, glm::vec3(0., 1., 0.));
    transform *= glm::rotate((r.z / glm::pi<float>()) * 180.f, glm::vec3(0., 0., 1.));
    transform *= glm::translate(position.x, position.y, position.z);
    std::cout << model->getAnimationFrameNumber(0) << std::endl;
    model->draw(renderer.GetShader(), transform, time);
  }
};
