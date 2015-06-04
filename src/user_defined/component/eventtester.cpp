#include <iostream>
#include "user_defined/component/eventtester.hh"
#include "ctvty/component/transform.hh"
#include "ctvty/debug.hpp"
#include "ctvty/component/hud.hh"

REGISTER_FOR_SERIALIZATION(user_defined::component, EventTester);

namespace user_defined {
  namespace component {
    std::chrono::time_point<std::chrono::system_clock> begin;

    EventTester::		EventTester()
      : MonoBehaviour(nullptr, "EventTester") {
      begin = std::chrono::high_resolution_clock::now();
      RegisterListener("exit click", &EventTester::ExitClick);
      RegisterListener("text click", &EventTester::TextClick);
    }


    EventTester::		EventTester(const serialization::Archive&)
      : MonoBehaviour(nullptr, "EventTester") {
      begin = std::chrono::high_resolution_clock::now();
      RegisterListener("exit click", &EventTester::ExitClick);
      RegisterListener("text click", &EventTester::TextClick);
    }

    void			EventTester::Serialize(serialization::Archive& __serial_instance) const {
      SERIALIZE_OBJECT_AS(user_defined::component::EventTester, __serial_instance);
      (void)__serial;
    }


    ctvty::Object*		EventTester::clone() const {
      return new EventTester();
    }

    void			EventTester::Update() {
      ctvty::component::Canvas* canvas = GetComponent<ctvty::component::Canvas>();
      if (canvas->Exist("text")) {
	serialization::Serial json;

	json & static_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - begin).count();
	(*canvas)["text"]->SetText(json.Stringify().substr(0, json.Stringify().find('.')));
      }
    }

    void			EventTester::Awake() {
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

    void		EventTester::ExitClick(ctvty::component::Hud* hud) {
      hud->disable();
      (hud->GetCanvas())["exit overed"]->enable();
      std::cout << "Exit Click" << std::endl;
    }

    void		EventTester::TextClick(ctvty::component::Hud* hud) {
      serialization::Serial json;
      json & static_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - begin).count();
      hud->SetText(json.Stringify());
    }

    void		EventTester::Render() {
    }
  };
};
