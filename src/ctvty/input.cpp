#include "ctvty/input.hh"

REGISTER_FOR_SERIALIZATION(ctvty, Input);
REGISTER_FOR_SERIALIZATION(ctvty::Input, configuration);

namespace ctvty {

  float				Input::GetKeyState(const std::string& key) {
    auto rbt_it = singleton()->config_map.find(key);
    if (rbt_it == singleton()->config_map.end())
      return 0.f;
    return rbt_it->second->value;
  }

  const utils::Vector3D&	Input::GetMousePosition() {
    return (singleton()->mouse);
  }

  void				Input::receiveEvent(const ctvty::Event* const e) {
    for (auto _pair : singleton()->config_map) {

      if (e->type() == Event::Type::keydown || e->type() == Event::Type::keyup)
	if (_pair.second->associated == configuration::type::key && _pair.second->key == e->keycode())
	  _pair.second->value = (e->type() == Event::Type::keydown);

      if (e->type() == Event::Type::mousemotion) {
	if (_pair.second->associated == configuration::type::mouseXaxis)
	  _pair.second->value = e->delta().x;
	if (_pair.second->associated == configuration::type::mouseYaxis)
	  _pair.second->value = e->delta().y;
	if (_pair.second->associated == configuration::type::mousex)
	  _pair.second->value = e->position().x;
	if (_pair.second->associated == configuration::type::mousey)
	  _pair.second->value = e->position().y;
      }

      if (e->type() == Event::Type::mousebuttondown || e->type() == Event::Type::mousebuttonup)
	if (_pair.second->associated == configuration::type::button && _pair.second->key == e->button())
	  _pair.second->value = (e->type() == Event::Type::mousebuttondown);
    }
    if (e->type() == Event::Type::mousemotion) {
      singleton()->mouse = e->position();
    }
  }


  void				Input::AssignInput(const std::string& s, const ctvty::Event* const e) {
    configuration* c = new configuration;

    if (e->type() == Event::Type::keydown || e->type() == Event::Type::keyup) {
      c->associated = configuration::type::key;
      c->key = e->keycode();
      c->value = (e->type() == Event::Type::keydown);
    }
    
    if (e->type() == Event::Type::mousebuttonup || e->type() == Event::Type::mousebuttondown) {
      c->associated = configuration::type::button;
      c->key = e->button();
      c->value = (e->type() == Event::Type::mousebuttondown);
    }
    if (singleton()->config_map[s])
      singleton()->config_map[s]->base_names.remove_if([s](const std::string& _s) -> bool { return _s == s; });
    singleton()->config_map[s].reset(c);
    c->base_names.push_back(s);
  }

  Input::			Input(const serialization::Archive& __serial)
    : mouse(ctvty::utils::Vector3D::zero) {
    std::list< std::shared_ptr<configuration> > tmp;

    __serial["config"] & tmp;
    for (auto& shared : tmp) {
      for (std::string name : shared->base_names)
	config_map[name] = shared;
    }

  }

  void				Input::Serialize(serialization::Archive& __serial_instance) const {
    std::list < const configuration* > tmp;
    SERIALIZE_OBJECT_AS(ctvty::Input, __serial_instance);
    
    for (auto& pair : config_map)
      tmp.push_back(pair.second.get());
    tmp.sort(); tmp.unique();
    __serial["config"] & tmp;
  }

  Input::configuration::	configuration(const serialization::Archive& __serial) {
    __serial["names"] & base_names;

    if (__serial.exist("mouse axis") &&
	(__serial["mouse axis"].as<std::string>() == "X" || __serial["mouse axis"].as<std::string>() == "x"))
      associated = type::mouseXaxis;
    else if (__serial.exist("mouse axis"))
      associated = type::mouseYaxis;

    if (__serial.exist("mouse position") &&
	(__serial["mouse position"].as<std::string>() == "X" || __serial["mouse position"].as<std::string>() == "x"))
      associated = type::mousex;
    else if (__serial.exist("mouse position"))
      associated = type::mousey;

    if (__serial.exist("key")) {
      associated = type::key;
      __serial["key"] & key;
    }

    if (__serial.exist("button")) {
      associated = type::button;
      __serial["button"] & key;
    }

    value = 0.f;
  }

  void				Input::configuration::Serialize(serialization::Archive& __serial_instance) const {
    SERIALIZE_OBJECT_AS(ctvty::Input::Serialization, __serial_instance);

    switch (associated) {
    case type::mouseXaxis:
      __serial["mouse axis"] & static_cast<const std::string>("x");
      break;
    case type::mouseYaxis:
      __serial["mouse axis"] & static_cast<const std::string>("y");
      break;
    case type::mousex:
      __serial["mouse position"] & static_cast<const std::string>("x");
      break;
    case type::mousey:
      __serial["mouse position"] & static_cast<const std::string>("y");
      break;
    case type::key:
      __serial["key"] & key;
      break;
    case type::button:
      __serial["button"] & key;
      break;
    };
  }
};
