#ifndef GdlCamera_hh__
# define GdlCamera_hh__

#include <cstdlib>

namespace GdlImpl {

  class Renderer;
  class Camera : public ctvty::rendering::Camera {
  private:
    Renderer&	renderer;
  public:
    Camera(Renderer&);


  private:
    ctvty::utils::Vector3D	position;
    ctvty::utils::Vector3D	lookAt;
    ctvty::utils::Quaternion	rotation;
  public:
    void			SetCameraPosition(const ctvty::utils::Vector3D& position,
						  const ctvty::utils::Vector3D& lookAt,
						  const ctvty::utils::Quaternion& rotation
						  = ctvty::utils::Quaternion::identity) final;
  public:
    class ViewPort {
    public:
      std::size_t baseX, baseY, width, height;
    };
  private:
    ViewPort	view;
    bool	autoViewPort;
    int		priority;
  public:
    void			SetViewPort(std::size_t baseX, std::size_t baseY,
					    std::size_t width, std::size_t height) final;
    void			DetectViewPort(int priority) final;
    void			DetectViewPort();
    ViewPort&			GetViewPort();

  public:
    inline ctvty::utils::Vector3D&	GetPosition() { return position; }
    inline ctvty::utils::Vector3D&	GetLookAt() { return lookAt; }
    inline ctvty::utils::Quaternion&	GetRotation() { return rotation; }
  };
};

#endif
