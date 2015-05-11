#include <iostream>

#include "gdlimpl/renderer.hh"

#include "ctvty/debug.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ctvty {
  namespace rendering {

    Renderer::Implementation&	Renderer::GetRenderer() {
      static ::GdlImpl::Renderer	renderer;
      return renderer;
    }
  };
};

namespace GdlImpl {
  void		Renderer::Initialize(std::size_t w,
				     std::size_t h,
				     const std::string& window_name) {
    width = w;
    height = h;
    gdl::SdlContext::start(w, h, window_name);
    _shader.load("./renderer/libraries/GdlLibrary/shaders/basic.fp", GL_FRAGMENT_SHADER);
    _shader.load("./renderer/libraries/GdlLibrary/shaders/basic.vp", GL_VERTEX_SHADER);
    _shader.build();
  }

  void		Renderer::Update() {
    _shader.bind();
  }

  ctvty::rendering::Camera*
		Renderer::CreateCamera() {
    Camera* camera = new Camera(*this);
    cameras.push_back(camera);
    return camera;
  }

  void		Renderer::UnregisterCamera(Camera* camera) {
    cameras.remove_if([&camera](Camera* _compare) {return _compare == camera;});
  }

  std::size_t	Renderer::RegisteredCameras() {
    return cameras.size();
  }

  void		Renderer::Pre3DRendering(int camera_id) {
    Camera* camera = nullptr;
    if (cameras.size()) {
      std::list<Camera*>::iterator it = cameras.begin();
      std::advance(it, camera_id);
      camera = *it;
      camera->DetectViewPort();
      glViewport(camera->GetViewPort().baseX,
		 camera->GetViewPort().baseY,
		 camera->GetViewPort().width,
		 camera->GetViewPort().height);
    }

    {
      float per;
      if (camera != nullptr)
	per = camera->GetViewPort().width / camera->GetViewPort().height;
      else
	per = width / height;
      glm::mat4 projection =
	glm::perspective(60.0f,
			 per,
			 0.1f, 100.0f);
      _shader.setUniform("projection", projection);
    }

    {
      glm::mat4 transformation;
      if (camera == nullptr)
	transformation
	  = glm::lookAt(
			glm::vec3(camera_position.x, camera_position.y, camera_position.z),
			glm::vec3(camera_lookAt.x, camera_lookAt.y, camera_lookAt.z),
			glm::vec3(camera_up.x, camera_up.y, camera_up.z)
			);
      else {
	transformation
	  = glm::lookAt(
			glm::vec3(camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z),
			glm::vec3(camera->GetLookAt().x, camera->GetLookAt().y, camera->GetLookAt().z),
			glm::vec3(camera->GetRotation().RotatedVector(ctvty::utils::Vector3D::up).x,
				  camera->GetRotation().RotatedVector(ctvty::utils::Vector3D::up).y,
				  camera->GetRotation().RotatedVector(ctvty::utils::Vector3D::up).z)
			);
      }
      
      _shader.setUniform("view", transformation);
    }

    glClear(GL_DEPTH_BUFFER_BIT);
  }

  void		Renderer::PreHUDRendering() {
    glm::mat4 projection =
      glm::ortho(0.0f,
		 static_cast<float>(width), static_cast<float>(height), 0.0f,
		 -1.0f, 1.0f);
    _shader.setUniform("projection", projection);
    _shader.setUniform("view", glm::mat4(1));
  }

  void		Renderer::Flush() {
    gdl::SdlContext::flush();
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void		Renderer::Quit() {
    gdl::SdlContext::stop();
  }

  void		Renderer::SetDefaultCameraPosition(const ctvty::utils::Vector3D& p,
						   const ctvty::utils::Vector3D& l,
						   const ctvty::utils::Quaternion& e) {
    camera_position = p;
    camera_lookAt = l;
    camera_up = e.RotatedVector(ctvty::utils::Vector3D::up);
  }

  ctvty::rendering::Model3D*
		Renderer::Load3DModel(const std::string& path) {
    return new Model3D(*this, path);
  }
};
