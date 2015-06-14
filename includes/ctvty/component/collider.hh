#ifndef Collider_hh__
# define Collider_hh__

# include "ctvty/component/rigidbody.hh"
# include "ctvty/utils/collision.hh"
# include "ctvty/physics/material.hh"

namespace ctvty {
  namespace component {

    class RigidBody;
    class Collider : public MonoBehaviour<Collider> {
    protected:
      utils::BoundingBox3D				boundingBox;
      RigidBody*					rigidBody;
      bool						isTrigger;
      std::shared_ptr<physics::Material>		material;

    private:
      std::list< RigidBody* >				triggereds;
      std::list< RigidBody* >				collideds;
    public:
      inline std::list< RigidBody* >&			Triggereds() { return triggereds; }
      inline std::list< RigidBody* >&			Collideds() { return collideds; }

    public:
      const RigidBody*			GetRigidBody() const;

    public:
      inline bool&			trigger() { return isTrigger; }

    public:
      bool				IsTrigger() const;
      const utils::BoundingBox3D&	GetBoundingBox() const;
      const physics::Material&		GetMaterial() const;

    public:
      void				Awake();
      void				OnDestroy();

    public:
      virtual const std::list<utils::Vector3D>&
					GetVertices() const = 0;

    protected:
      virtual ctvstd::Optional<utils::Collision>
					CollisionImpl(const Collider*		contact_collider,
						      const utils::Vector3D&	position,
						      const utils::Quaternion&	quaternion,
						      const utils::Vector3D&	direction) const = 0;

    public:
      ctvstd::Optional<utils::Collision>
					Collision(const std::list<Collider*>&	contact_colliders,
						  const utils::Vector3D&	position,
						  const utils::Quaternion&	quaternion,
						  const utils::Vector3D&	direction) const;

      virtual ctvstd::Optional<float>		RayCollision(const utils::Vector3D& origin,
							     const utils::Vector3D& direction) const = 0;

    protected:
      Collider(GameObject* parent, const std::string&);
    };
    
  };
};

#endif
