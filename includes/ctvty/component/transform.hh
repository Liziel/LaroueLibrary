#ifndef Transform_hh__
# define Transform_hh__

# include "ctvty/monobehaviour.hpp"
# include "ctvty/utils/vector3d.hh"

namespace ctvty {
  namespace component {
    
    /*
     * the transform component that all gameObject contain
     */
    class Transform : public MonoBehaviour<Transform>{
    private:
      std::shared_ptr<utils::Vector3D>		scale;
      std::shared_ptr<utils::Vector3D>		position;
      std::shared_ptr<utils::Quaternion>	rotation;

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
      void		Serialize(serialization::Archive& __serial) const override;

    public:
      Object*		clone() const override;

    public:
      void		SetPosition(const utils::Vector3D&);
      void		Translate(const utils::Vector3D&);

    public:
      void		SetEulerRotation(const utils::Vector3D&);
      void		EulerRotate(const utils::Vector3D&);
      void		Rotate(const utils::Quaternion&);

    public:
      const utils::Vector3D	GetRealPosition() const;
      const utils::Quaternion	GetRealRotation() const;

    public:
      const utils::Vector3D&	GetPosition() const;
      const utils::Quaternion&	GetRotation() const;
      const utils::Vector3D&	GetScale() const;

    public:
      utils::Vector3D&		GetPosition();
      utils::Quaternion&	GetRotation();
      utils::Vector3D&		GetScale();

    };

  };
};

#endif
