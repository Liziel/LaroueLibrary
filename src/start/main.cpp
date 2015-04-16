#include <iostream>
#include "serialization/serializable.hh"
#include "ctvty/gameObject.hpp"
#include "ctvty/event/clock.hh"

int main() {
  ctvty::GameObject	*Army = new ctvty::GameObject("Army", "army", nullptr, true);
  serialization::Serial	json;

  Army->AddChild(new ctvty::GameObject("Soldier1", "soldier", nullptr, true));
  Army->AddChild(new ctvty::GameObject("Soldier2", "soldier", nullptr, true));
  Army->AddChild(new ctvty::GameObject("Soldier3", "soldier", nullptr, true));
  Army->AddChild(new ctvty::GameObject("Soldier4", "soldier", nullptr, true));

  json & Army;

  std::cout << json.Stringify() << std::endl;

  ctvty::Object::Destroy( Army );

  ctvty::Object::Destroy( (ctvty::Object*)serialization::Serializable::Instantiate( json ) );

  new ctvty::event::DelayedAction(2.5f, [](){std::cout << "yeah" << std::endl;});
  new ctvty::event::DelayedAction(5.f, [](){exit(0);});

  ctvty::event::Clock::GetClock().Start();
}
