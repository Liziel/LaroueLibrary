#ifndef Rigidbody_hh__
# define Rigidbody_hh__

# include "ctvty/monobehaviour.hpp"
# include "ctvty/utils/vector3d.hh"
# include "ctvty/utils/boundingbox.hh"

namespace ctvty {
  namespace component {
    
    enum class CollisionDetectionMode {
      Discrete,//will do a low check for when the body is moving, use for a low speed moving object (a player)
      ContinousDynamic,
      Continous
    };

    enum class ForceMode {
      Force,//continous force, using RigidBody mass --> ex : someone running and jumping at the same time
      Acceleration,//continuous force, non-using RigidBody mass --> ex : climbing a ladder
      Impulse,//Instant force, using RigidBody mass --> ex: a jump
      VelocityChange//Instant force, non-using RigidBody mass --> ex : a jetpack (or somewhat)
    };

    class Collider;
    class RigidBody : public MonoBehaviour<RigidBody> {
    private:
      utils::BoundingBox3D			boundingBox;
      std::list<Collider*>			sub_colliders;

    private:
      std::list<Collider*>			colliders_collisions;

    private:
      std::list<Collider*>			colliders_trigger;

    private://later accessible and serializable
      CollisionDetectionMode			detectionMode;
      bool					detectCollision;
      bool					isKinematic;
      float					mass;
      std::shared_ptr<utils::Vector3D>		velocity;
      
    private:
      bool			asleep;

    public:
      void			AddExplosionForce(float explosionForce, utils::Vector3D explosionPosition,
						  float explosionRadius);
      void			AddForce(utils::Vector3D force, ForceMode mode);

    public:
      bool			IsSleeping();
      void			WakeUp();

    public:
      utils::Vector3D&		GetVelocity();

    public:
      /*
       * accessed only by MonoBehaviour
       */
      void			Awake();
      void			FixedUpdate();

    public:
      void			RegisterCollider(Collider*);

    public:
      RigidBody(GameObject* parent);
      RigidBody(const serialization::Archive&);

    public:
      void	Serialize(serialization::Archive& __serial) const override;

    public:
      Object*	clone() const override;

    private:
      void			DiscreteCheckMovement(utils::Vector3D&);
      void			ContinousCheckMovement(utils::Vector3D&);
      void			DynamicContinousCheckMovement(utils::Vector3D&);
    };

  };
};

#endif
