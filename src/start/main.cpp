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

void	recursiveList(filesystem::Directory& directory) {
  std::list<filesystem::Directory>	subList;

  if (directory) {
    std::cout << directory.GetPath() << ":" << std::endl;
    for (const filesystem::File& file : directory) {
      std::cout << file.GetName() << " ";
      if (file.GetName() != "." && file.GetName() != ".." && file.IsDirectory())
	subList.emplace_back(directory.GetPath() + '/' + file.GetName());
    }
    std::cout << std::endl << std::endl;
    std::for_each(subList.begin(), subList.end(),
		  &recursiveList);
  }
}

using namespace ctvty::utils;
int main(int ac, char** av) {
  std::shared_ptr<ctvty::asset::Cache<ctvty::GameObject>>
					save_cache;

  ctvty::asset::Assets			assets("assets");
  ctvty::asset::Assets			save_assets(std::move(assets.GetAssets("save")));

  ctvty::GameObject			*Army = nullptr;

  if (1)
    {
      bool	initialization_succes;

      if (ac == 2)
	initialization_succes = ctvty::Application::Initialize(av[1]);
      else
	initialization_succes = ctvty::Application::Initialize("assets");
      if (!initialization_succes)
	return (-1);
      ctvty::Application::Quit(60.f);
      ctvty::Application::Start();
    }


  if (0)
    {
      Army  = new ctvty::GameObject("Falling Cube", "cubes", nullptr,
				    new ctvty::utils::Vector3D(ctvty::utils::Vector3D::forward),
				    new ctvty::utils::Quaternion(0.f, 0.f, 0.f, 0.f));
      Army->AddComponent<ctvty::component::RigidBody>();
      Army->AddComponent<ctvty::component::BoxCollider>();
      save_assets.GetAsset("falling_cube.json").Save(Army);
    }

  if (0)
    {
      ctvty::debug::CompressedLogs(Quaternion::Euler(Vector3D::up * M_PI / 2), 5, 4, new Vector3D(Vector3D::up));
      ctvty::debug::Logs(Quaternion::Euler(Vector3D::up * M_PI / 2), 5, 4, new Vector3D(Vector3D::up));
    }

  if (0)
    {
      Quaternion	q(Quaternion::Euler(Vector3D::up * M_PI / 2));
      serialization::Serial json;

      json & &q;
      std::cout << json.Stringify() << std::endl;

      ctvty::utils::Vector3D	direction(Vector3D::forward * 10 + Vector3D::up);
      direction = q.RotatedAround(Vector3D::up, direction);
      json & &direction;
      std::cout << json.Stringify() << std::endl;

    }

  if (0)
    {
      ctvty::utils::Face face({
	  ctvty::utils::Vector3D::zero,
	    ctvty::utils::Vector3D::right,
	    ctvty::utils::Vector3D::up +
	    ctvty::utils::Vector3D::right,
	    ctvty::utils::Vector3D::up,
	    });
      serialization::Serial json;

      ctvty::utils::Vector3D	point(Vector3D::back * 2 + Vector3D::up/2);
      ctvty::utils::Vector3D	direction(Vector3D::forward * 10);
      float			force;

      json & &(point);
      std::cout << json.Stringify() << std::endl;
      json & &(direction);
      std::cout << json.Stringify() << std::endl;

      { Vector3D tmp = point + direction * 0.5;
	json & &(tmp);
	std::cout << json.Stringify() << std::endl;}

      std::cout << std::boolalpha << std::endl;
      ctvstd::Optional<ContactPoint> p = face.Intersection(point, direction, force);
      std::cout << "collide ? " << p << std::endl;
      if (p) {
	std::cout << "at" << "{ " << p->point.x << " , " << p->point.y << " , " << p->point.z << " }" << std::endl;
	std::cout << "with force " << force << std::endl;
      }
    }

  if (0)
    {
      ctvty::Application::Quit(5.f);
      new ctvty::event::DelayedAction([] () {ctvty::Application::LoadScene("base");}, 1.5f);
      new ctvty::event::DelayedAction([] () {ctvty::Application::LoadScene("complex");}, 3.f);
      ctvty::Application::Start();
    }

  if (0)
    {
      std::cout << "assets are cached" << std::endl;
      save_cache.reset(new ctvty::asset::Cache<ctvty::GameObject>([](ctvty::asset::Asset& file)->bool {
	    bool	pass = file.GetFile().GetName()
	      .substr(file.GetFile().GetName().size()-sizeof("json") + 1, sizeof("json")) == "json";
	    pass = pass && (file.GetFile().GetName() != "ArmyArray.json");
	    if (pass)
	      file.SetDeleter([](serialization::Serializable* obj) {
		  if (dynamic_cast<ctvty::Object*>(obj) != nullptr)
		    ctvty::Object::Destroy(dynamic_cast<ctvty::Object*>(obj));
		});
	    return pass;
	  }));
      save_assets.Cache(save_cache.get());
    }

  if (0)
    {
      std::cout << "Army will be saved" << std::endl;
      save_assets.GetAsset("Army.json").Save(Army);
    }

  if (0)
    {
      std::cout << "some copy are made" << std::endl;
      std::shared_ptr<ctvty::GameObject>	ArmyCopy1 = save_assets.GetAsset("Army.json").LoadAs<ctvty::GameObject>();
      std::shared_ptr<ctvty::GameObject>	ArmyCopy2 = save_assets.GetAsset("Army.json").LoadAs<ctvty::GameObject>();
      std::shared_ptr<ctvty::GameObject>	ArmyCopy3 = save_assets.GetAsset("Army.json").LoadAs<ctvty::GameObject>();
    }

  if (0)
    {
      filesystem::Directory root(".");
      recursiveList(root);
    }

  if (0)
    {
      std::cout << "call to Destroy" << std::endl;
      ctvty::Object::Destroy(Army);
    }
}
