#ifndef Component_Camera_hh__
# define Component_Camera_hh__

# include "ctvty/monobehaviour.hpp"
# include "ctvty/rendering/camera.hh"

namespace ctvty {
  namespace component {

    class Camera : public MonoBehaviour<Camera> {
    public:
      class View : serialization::Serialzable {
      public:
	std::size_t	x, y, width, height;
	
      public:
	View(const serialization::Archive&);
	void	Serialize(serialization::Archive&) const override;
      };
    private:
      std::shared_ptr<View>
				defined_view;
      int			priority;

    private:
      std::shared_ptr<ctvty::rendering::Camera>
				camera;

    private:
      std::shared_ptr<ctvty::utils::Vector3D>
				offset;

    private:
      std::shared_ptr<ctvty::utils::Vector3D>
				lookAt;
      bool			lookForward;      

    public:
		Camera();
		Camera(const serialization::Archive&)
      void	Serialize(serialization::Archive&) const override;

    public:
      Object*	clone() const override;

    public:
      inline
      ctvty::utils::Vector3D&	LookAt() { return lookAt; }
      inline
      void			LookForward(bool cond) { lookForward = cond; }

    public:
      inline
      ctvty::utils::Vector3D&	GetOffset() { return offset; }

    public:
      void	Awake();
      void	Update();
    };
  };
};


#endif
