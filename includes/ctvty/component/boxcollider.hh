#ifndef BoxCollider_hh__
# define BoxCollider_hh__

# include "ctvty/component/collider.hh"

namespace ctvty {
  namespace component {

    class BoxCollider : public Collider {
    private:
      utils::Vector3D*				base_offset;
      utils::Vector3D*				size;
      float				scale;

    public:
      BoxCollider(const utils::Vector3D& base_offset, const	utils::Vector3D& size, float scale = 1.);
      BoxCollider(	utils::Vector3D* base_offset,		utils::Vector3D* size, float scale = 1.);
      ctvty::Object*			clone() const;

    public:
					BoxCollider(const serialization::Archive&);
      void				Serialize(serialization::Archive&);

    public:
      std::list<utils::Vector3D>&	GetVertices() const;

    public:
      ctvstd::Optional<utils::Collision>
					Collision(const std::list<Collider*>& contact_colliders,
						  const utils::Vector3D& position,
						  const utils::Vector3D& direction);

    };

  };
};
#endif
