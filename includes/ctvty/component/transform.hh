#ifndef Transform_hh__
# define Transform_hh__

# include "ctvty/monobehaviour.hpp"

namespace ctvty {
  namespace component {
    
    /*
     * the transform component that all gameObject contain
     */
    class Transform : public MonoBehaviour<Transform>{
    private:
      utils::Vector3D*		scale;
      utils::Vector3D*		position;
      utils::Quaternion*	rotation;

    private:
      Transform*		parent;
      std::list<Transform*>	childs;
    public:
      void		SetParent(Transform*);

    public:
      Transform(GameObject* gameObject,
		const utils::Vector3D& position, const utils::Quaternion& rotation,
		const utils::Vector3D& scale = utils::Vector3D(1.0, 1.0, 1.0));
      Transform(GameObject* gameObject,
		utils::Vector3D* position, utils::Quaternion* rotation,
		utils::Vector3D* scale = new utils::Vector3D(1.0, 1.0, 1.0));
      Transform(const serialization::Archive&);

    public:
      ~Transform();

    public:
      void		Serialize(serialization::Archive& __serial) override;

    public:
      Object*		clone() const;

    public:
      void		Translate(const utils::Vector3D&);
      void		Rotate(const utils::Vector3D&);

    public:
      utils::Vector3D&	GetPosition();

    };

  };
};

#endif
