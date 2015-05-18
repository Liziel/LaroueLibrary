#include "SDL.h"
#include "ctvty/event.hh"

namespace ctvty {

  std::shared_ptr<Event>&		Event::setcurrent() {
    static std::shared_ptr<Event>	_current;

    return _current;
  }

  const std::shared_ptr<Event>&		Event::current() {
    return setcurrent();
  }

  std::queue<Event*>			Event::events({});

  void					Event::Refresh() {
    SDL_Event	e;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
	events.push(new Event());
      else if (e.type == SDL_KEYDOWN)
	events.push(new Event(e.key.keysym.scancode, e.key.keysym.sym, true));
      else if (e.type == SDL_KEYUP)
	events.push(new Event(e.key.keysym.scancode, e.key.keysym.sym, false));
      else if (e.type == SDL_MOUSEMOTION)
	events.push(new Event(utils::Vector3D(e.motion.xrel, e.motion.yrel, 0),
			      utils::Vector3D(e.motion.x, e.motion.y, 0)));
      else if (e.type == SDL_MOUSEBUTTONUP)
	events.push(new Event(e.button.button, false,
			      utils::Vector3D(e.button.x, e.button.y, 0)));
      else if (e.type == SDL_MOUSEBUTTONDOWN)
	events.push(new Event(e.button.button, true,
			      utils::Vector3D(e.button.x, e.button.y, 0)));
    }
  }

  Event*				Event::Eat() {
    Event* e = events.front(); events.pop();
    setcurrent().reset(e);
    return e;
  }


  Event::Event()
    : _type(Type::quit), _button(0), _character(0), _keycode(0) {
  }

  Event::Event(char c, int k, bool s)
    : Event(){
    _type = (s ? Type::keydown : Type::keyup);
    _character = c;
    _keycode = k;
  }

  Event::Event(const utils::Vector3D& d, const utils::Vector3D& p) {
    _type = Type::mousemotion;
    _delta = d;
    _position = p;
  }

  Event::Event(int b, bool s, const utils::Vector3D& p) {
    _type = (s ? Type::mousebuttondown : Type::mousebuttonup);
    _position = p;
    _button = b;
  }

};
