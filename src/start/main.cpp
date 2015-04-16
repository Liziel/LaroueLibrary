#include <iostream>
#include "serialization/serializable.hh"
#include "filesystem/directory.hh"
#include "ctvty/gameObject.hpp"
#include "ctvty/event/clock.hh"


void	recursiveList(const std::string& _directory) {
  filesystem::Directory	directory(_directory);

  if (directory)
    for (auto file : directory) {
      std::cout << "path: \"" << file.GetPath() << "\", name: " << file.GetName() << std::endl;
      if (file.GetName() != "." && file.GetName() != "..")
	recursiveList(file.GetPath());
    }
}

int main() {
  ctvty::GameObject	*Army = new ctvty::GameObject("Army", "army", nullptr, true);
  serialization::Serial	json;

  Army->AddChild(new ctvty::GameObject("Soldier1", "soldier", nullptr, true));
  Army->AddChild(new ctvty::GameObject("Soldier2", "soldier", nullptr, true));
  Army->AddChild(new ctvty::GameObject("Soldier3", "soldier", nullptr, true));
  Army->AddChild(new ctvty::GameObject("Soldier4", "soldier", nullptr, true));

  json & Army;

  std::cout << json.Stringify() << std::endl;

  recursiveList(".");

  new ctvty::event::DelayedAction(2.5f, [](){std::cout << "yeah" << std::endl;});
  new ctvty::event::DelayedAction(5.f, [](){ctvty::event::Clock::GetClock().End();});

  auto start = std::chrono::high_resolution_clock::now();
  ctvty::event::Clock::GetClock().Start();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;
  std::cout << diff.count() << " s" << std::endl;

  ctvty::Object::Destroy( Army );

  ctvty::Object::Destroy( (ctvty::Object*)serialization::Serializable::Instantiate( json ) );

}
