#ifndef BoxCollider_hh__
# define BoxCollider_hh__

# include "ctvty/component/collider.hh"

namespace ctvty {
  namespace component {

    class BoxCollider : public Collider {
    private:
      std::list<utils::Vector3D>			vertices;
      std::array<std::shared_ptr<utils::Face>, 6>	faces;

    private:
      std::shared_ptr<utils::Vector3D>			msize;
      std::shared_ptr<utils::Vector3D>			psize;
      float				scale;

    public:
      BoxCollider(GameObject*		gameObject);
      ctvty::Object*			clone() const;

    public:
					BoxCollider(const serialization::Archive&);
      void				Serialize(serialization::Archive&) const;

    private:
      void				CalculateVertices();
    public:
      const std::list<utils::Vector3D>&	GetVertices() const;

    public:
      void				Scale(float);

    private:
      ctvstd::Optional<utils::Collision>
					CollisionImpact(const utils::Vector3D& vertex,
							const utils::Vector3D& direction) const;

    private:
      ctvstd::Optional<utils::Collision>
					CollisionIter(std::list<utils::Vector3D>::const_iterator b,
						      std::list<utils::Vector3D>::const_iterator e,
						      const utils::Vector3D& position,
						      const utils::Quaternion& roatation,
						      const utils::Vector3D& direction) const;

    public:
      ctvstd::Optional<utils::Collision>
					CollisionImpl(const Collider* contact_colliders,
						      const utils::Vector3D& position,
						      const utils::Quaternion& roatation,
						      const utils::Vector3D& direction) const override;

    };

  };
};
#endif
