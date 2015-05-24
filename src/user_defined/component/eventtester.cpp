#include <iostream>
#include "user_defined/component/eventtester.hh"
#include "ctvty/component/transform.hh"
#include "ctvty/debug.hpp"
#include "ctvty/component/rigidbody.hh"

namespace user_defined {
  namespace component {

    REGISTER_FOR_SERIALIZATION(user_defined::component, EventTester);


    EventTester::		EventTester()
      : MonoBehaviour(nullptr, "EventTester") {
    }


    EventTester::		EventTester(const serialization::Archive&)
      : MonoBehaviour(nullptr, "EventTester") {
    }

    void			EventTester::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::EventTester, __serial_instance);
      (void)__serial;
    }


    ctvty::Object*		EventTester::clone() const {
      return new EventTester();
    }

    void			EventTester::Update() {
    }

    void			EventTester::Awake() {
      texture.reset(ctvty::rendering::Renderer::GetRenderer().LoadTexture("assets/textures/exit.tga"));
      hud = ctvty::rendering::Renderer::GetRenderer().CreateHud();
      hud->SetTexture(texture);
      hud->SetPosition(0.8, 0.1 , 0.1, 0.1);
      hud->SetScreenSpace(0);
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
    }
  };
};
