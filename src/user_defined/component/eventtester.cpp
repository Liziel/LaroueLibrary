#include <iostream>
#include "user_defined/component/eventtester.hh"

namespace user_defined {
  namespace component {

    REGISTER_FOR_SERIALIZATION(user_defined::component, EventTester);


    EventTester::		EventTester()
      : MonoBehaviour(nullptr, "EventTester") {}


    EventTester::		EventTester(const serialization::Archive&)
      : MonoBehaviour(nullptr, "EventTester") {}

    void			EventTester::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::EventTester, __serial_instance);
      (void)__serial;
    }


    ctvty::Object*		EventTester::clone() const {
      return new EventTester();
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


  };
};
