#include <iostream>
#include "serialization/serializable.hh"
#include "filesystem/directory.hh"
#include "ctvty/gameObject.hpp"
#include "ctvty/event/clock.hh"
#include "ctvty/assets/assets.hh"

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

int main() {
  ctvty::asset::Cache<ctvty::GameObject>
					*save_cache;
  ctvty::asset::Assets			assets("assets");
  ctvty::asset::Assets			save_assets(std::move(assets.GetAssets("save")));
  ctvty::GameObject			*Army = new ctvty::GameObject("Army", "army", nullptr, true);

  Army->AddChild(new ctvty::GameObject("Soldier1", "soldier", nullptr, true));
  Army->AddChild(new ctvty::GameObject("Soldier2", "soldier", nullptr, true));
  Army->AddChild(new ctvty::GameObject("Soldier3", "soldier", nullptr, true));
  Army->AddChild(new ctvty::GameObject("Soldier4", "soldier", nullptr, true));


  std::cout << "Army will be saved" << std::endl;
  save_assets.GetAsset("Army.json").Save(Army);

  {
    std::cout << "assets are cached" << std::endl;
    save_cache = new ctvty::asset::Cache<ctvty::GameObject>([](ctvty::asset::Asset& file)->bool {
	bool	pass = file.GetFile().GetName()
	  .substr(file.GetFile().GetName().size()-sizeof("json") + 1, sizeof("json")) == "json";
	if (pass)
	  file.SetDeleter([](serialization::Serializable* obj) {
	      ctvty::Object::Destroy(dynamic_cast<ctvty::Object*>(obj));
	    });
	return pass;
      });
    assets.Cache(save_cache);
  }

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

  std::cout << "call to Destroy" << std::endl;
  ctvty::Object::Destroy(Army);

  std::cout << "delete cache" << std::endl;
  delete save_cache;
}
