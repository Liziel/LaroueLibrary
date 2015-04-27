#ifndef BoxCollider_hh__
# define BoxCollider_hh__

# include "ctvty/component/collider.hh"

namespace ctvty {
  namespace component {

    class BoxCollider : public Collider {
    private:
      std::list<utils::Vector3D>	vertices;

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

    public:
      ctvstd::Optional<utils::Collision>
					CollisionImpl(const Collider* contact_colliders,
						      const utils::Vector3D& position,
						      const utils::Vector3D& direction) override;

    };

  };
};
#endif
