#include <cmath>
#include <iostream>
#include "serialization/serializable.hh"
#include "filesystem/directory.hh"
#include "ctvty/gameObject.hpp"
#include "ctvty/application.hh"
#include "ctvty/event/clock.hh"
#include "ctvty/assets/assets.hh"
#include "ctvty/component/rigidbody.hh"
#include "ctvty/utils/face.hh"
#include "ctvty/debug.hpp"
#include "ctvty/component/boxcollider.hh"
#include "ctvty/rendering/renderer.hh"
#include "ctvty/component/renderer.hh"

using namespace ctvty::utils;
int main(int ac, char** av) {
  ctvty::asset::Assets			assets("assets");

  if (0)
    {
      bool	initialization_succes;

      std::cout << "before init" << std::endl;
      if (ac == 2)
	initialization_succes = ctvty::Application::Initialize(av[1]);
      else
	initialization_succes = ctvty::Application::Initialize("assets");
      if (!initialization_succes)
	return (-1);
      std::cout << "after init" << std::endl;
      ctvty::Application::Quit(5.f);
      ctvty::Application::Start();
    }

  if (1)
    {
      serialization::Serial json, jsont;
      std::map<std::string, std::shared_ptr<ctvty::GameObject> > map;

      map.emplace(std::string("falling cube"),
		  assets.GetAsset("save/falling_cube2.json").LoadAs<ctvty::GameObject>());
      json & map;
      std::cout << json.Stringify() << std::endl;
      map.clear();
      jsont & map;
      std::cout << jsont.Stringify() << std::endl;
      static_cast<const serialization::Serial&>(json) & map;
      jsont & map;
      std::cout << jsont.Stringify() << std::endl;
    }
}
