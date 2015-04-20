#ifndef Rigidbody_hh__
# define Rigidbody_hh__

# include "ctvty/monobehaviour.hpp"
# include "ctvty/utils/vector3d.hh"

namespace ctvty {
  namespace component {
    
    enum CollisionDetectionMode {
      discrete,//will do a low check for when the body is moving
      raycast,//will raycast to see if the body collision other box, great impact on performance
    };

    enum ForceMode {
      Force,//continous force, using RigidBody mass --> ex : someone running and jumping at the same time
      Acceleration,//continuous force, non-using RigidBody mass --> ex : climbing a ladder
      Impulse,//Instant force, using RigidBody mass --> ex: a jump
      VelocityChange//Instant force, non-using RigidBody mass --> ex : a jetpack (or somewhat)
    };

    class RigidBody : public MonoBehaviour<RigidBody> {
    private:
      CollisionDetectionMode	detectionMode;
      bool			detectCollision;
      bool			isKinematic;
      float			mass;
      utils::Vector3D*		velocity;
      
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
      void			Start();
      void			FixedUpdate();
      

    public:
      RigidBody(GameObject* parent);
      RigidBody(const serialization::Archive&);

    public:
      void	Serialize(serialization::Archive& __serial) override;

    public:
      Object*	clone() const override;
    };

  };
};

#endif
