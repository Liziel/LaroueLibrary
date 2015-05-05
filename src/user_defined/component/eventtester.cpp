#include <iostream>
#include "user_defined/component/eventtester.hh"
#include "ctvty/component/transform.hh"
#include "ctvty/debug.hpp"

namespace user_defined {
  namespace component {

    REGISTER_FOR_SERIALIZATION(user_defined::component, EventTester);


    EventTester::		EventTester()
      : MonoBehaviour(nullptr, "EventTester"),
	model3D(ctvty::rendering::Renderer::GetRenderer().Load3DModel("assets/models3d/Bombe Health/Health.FBX")) {}


    EventTester::		EventTester(const serialization::Archive&)
      : MonoBehaviour(nullptr, "EventTester"),
	model3D(ctvty::rendering::Renderer::GetRenderer().Load3DModel("assets/models3d/Bombe Health/Health.FBX")) {}

    void			EventTester::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::EventTester, __serial_instance);
      (void)__serial;
    }


    ctvty::Object*		EventTester::clone() const {
      return new EventTester();
    }

    void			EventTester::Update() {
      ctvty::debug::Logs(gameObject);
    }

    /*			Collision Events Tests			*/
    void		EventTester::OnCollisionEnter(const ctvty::utils::Collision*) {
      std::cerr << std::endl << std::endl << std::endl
		<< "On Collision Enter"
		<< std::endl << std::endl << std::endl;
      
    }

    void		EventTester::OnCollisionStay(const ctvty::utils::Collision*) {
      std::cerr << std::endl << std::endl << std::endl
		<< "On Collision Stay"
		<< std::endl << std::endl << std::endl;
    }

    void		EventTester::OnCollisionExit(const ctvty::utils::Collision*) {
      std::cerr << std::endl << std::endl << std::endl
		<< "On Collision Exit"
		<< std::endl << std::endl << std::endl;
    }


    void		EventTester::Render() {
      model3D->Draw(transform->GetPosition(),
		    ctvty::utils::Vector3D::one / 100,
		    ctvty::utils::Quaternion::identity);
    }
  };
};
