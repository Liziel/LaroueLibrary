#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gdlimpl/renderer.hh"
#include "gdlimpl/texture.hh"
#include "ctvty/debug.hpp"


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
    _next_shader_use = nullptr;
    width = w;
    height = h;
    gdl::SdlContext::start(w, h, window_name);
    _shader.load("./renderer/libraries/GdlLibrary/shaders/basic.fp", GL_FRAGMENT_SHADER);
    _shader.load("./renderer/libraries/GdlLibrary/shaders/basic.vp", GL_VERTEX_SHADER);
    _shader.build();
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glEnable (GL_TEXTURE);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (TTF_Init() != 0)
      std::cout << "TTF init: " << TTF_GetError() << std::endl;
  }

  gdl::BasicShader&			Renderer::GetShader() {
    if (_next_shader_use != nullptr) {
      gdl::BasicShader& _next = _next_shader_use->GetShader();
      _next_shader_use = nullptr;
      _next.bind();
      return _next;
    }
    _shader.bind();
    return _shader;
  }

  void		Renderer::Update() {
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
    std::list<Camera*>::iterator it = cameras.begin();
    std::advance(it, camera_id);
    camera = *it;
    camera->DetectViewPort();
    glViewport(camera->GetViewPort().baseX,
	       camera->GetViewPort().baseY,
	       camera->GetViewPort().width,
	       camera->GetViewPort().height);

    {
      float per =
	camera->GetViewPort().width / camera->GetViewPort().height;
      glm::mat4 projection =
	glm::perspective(60.0f,
			 per,
			 0.1f, 100.0f);
      _shader.setUniform("projection", projection);
      for (Shader* shader : shaders)
	shader->GetShader().setUniform("projection", projection);
    }

    {
      glm::mat4 transformation;
	transformation
	  = glm::lookAt(
			glm::vec3(camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z),
			glm::vec3(camera->GetLookAt().x, camera->GetLookAt().y, camera->GetLookAt().z),
			glm::vec3(camera->GetRotation().RotatedVector(ctvty::utils::Vector3D::up).x,
				  camera->GetRotation().RotatedVector(ctvty::utils::Vector3D::up).y,
				  camera->GetRotation().RotatedVector(ctvty::utils::Vector3D::up).z)
			);
      
      _shader.setUniform("view", transformation);
      for (Shader* shader : shaders)
	shader->GetShader().setUniform("view", transformation);
    }

    glClear(GL_DEPTH_BUFFER_BIT);
  }

  void		Renderer::PreHUDRendering(int camera_id) {
    Camera* camera = nullptr;
    std::list<Camera*>::iterator it = cameras.begin();
    std::advance(it, camera_id);
    camera = *it;
    camera->DetectViewPort();
    glViewport(camera->GetViewPort().baseX,
	       camera->GetViewPort().baseY,
	       camera->GetViewPort().width,
	       camera->GetViewPort().height);

    glm::mat4 projection =
      glm::ortho(0.0f,
		 static_cast<float>(camera->GetViewPort().width),
		 static_cast<float>(camera->GetViewPort().height), 0.0f,
		 -1.0f, 1.0f);
    _shader.setUniform("projection", projection);
    for (Shader* shader : shaders)
      shader->GetShader().setUniform("projection", projection);
    _shader.setUniform("view", glm::mat4(1));
    for (Shader* shader : shaders)
      shader->GetShader().setUniform("view", glm::mat4(1));
    glClear(GL_DEPTH_BUFFER_BIT);
  }

  void		Renderer::MainHUDRendering() {
    glViewport(0.f, 0.f,
	       width, height);
    glm::mat4 projection =
      glm::ortho(0.0f,
		 static_cast<float>(width), static_cast<float>(height), 0.0f);
    _shader.setUniform("projection", projection);
    for (Shader* shader : shaders)
      shader->GetShader().setUniform("projection", projection);
    _shader.setUniform("view", glm::mat4(1));
    for (Shader* shader : shaders)
      shader->GetShader().setUniform("view", glm::mat4(1));
    glClear(GL_DEPTH_BUFFER_BIT);
    for (auto& huds : screenhuds) {
      for (auto it = huds.second.begin();
	   huds.second.end() != it; ++it) {
	if (!it->expired())
	  it->lock()->Draw();
	else
	  it = huds.second.erase(it);
      }
      glClear(GL_DEPTH_BUFFER_BIT);
    }
  }

  void		Renderer::Flush() {
    _shader.bind();
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

  ctvty::rendering::Texture*
		Renderer::LoadTexture(const std::string& path) {
    return new Texture(path);
  }

  std::shared_ptr<ctvty::rendering::Hud>
		Renderer::CreateHud() {
    std::shared_ptr<Hud> n ( new Hud(*this, &inactivesHuds) );
    n->SetWeakRef(n);
    inactivesHuds.push_back(n);
    return n;
  }

  std::shared_ptr<ctvty::rendering::Shader>
		Renderer::CreateShader(const std::string& a, const std::string& b) {
    Shader* shader = new Shader(*this, a, b);
    shaders.push_back(shader);
    return  std::shared_ptr<ctvty::rendering::Shader>{ shader };
  }

  void		Renderer::UseShaderAtNextDraw(std::shared_ptr<ctvty::rendering::Shader> shader) {
    _next_shader_use = std::dynamic_pointer_cast<Shader>(shader);
  }

  void		Renderer::UnsetShader(GdlImpl::Shader* shader) {
    shaders.remove_if([shader](GdlImpl::Shader* __shader) { return __shader == shader; });
  }
};
