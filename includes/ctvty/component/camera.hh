#ifndef Component_Camera_hh__
# define Component_Camera_hh__

# include "ctvty/monobehaviour.hpp"
# include "ctvty/rendering/camera.hh"

namespace ctvty {
  namespace component {

    class Camera : public MonoBehaviour<Camera> {
    public:
      class View : public serialization::Serializable {
      public:
	std::size_t	x, y, width, height;
	
      public:
		View(std::size_t,std::size_t,std::size_t,std::size_t);
		View(const serialization::Archive&);
	void	Serialize(serialization::Archive&) const override;
      };
    public:
      enum class view {
	automatic,//will automaticly detect the view port needed by its priority
	  defined,//the view port correspond toits user definition
	  global//the view port is of the size of the window
      };
    private:
      std::shared_ptr<View>	view;
      int			priority;
      enum view			vtype;
    private:
      std::shared_ptr<ctvty::rendering::Camera>
				camera;

    private:
      std::shared_ptr<ctvty::utils::Vector3D>
				offset;

    public:
      enum class look{
	forward,//will look forward... the rotation of the gameObject will be applied
	  locked,//will always look at the indicated position
	  free,//will look at the indicated position + the position of the gameObject
      };
    private:
      std::shared_ptr<ctvty::utils::Vector3D>
				lookAt;
      enum look			ltype;


		Camera();
    public:
		Camera(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const override;

    public:
      Object*	clone() const override;

    public:
      void	Awake();
      void	Update();
    };
  };
};


#endif
