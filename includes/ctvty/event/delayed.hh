#ifndef Delayed_hh__
# define Delayed_hh__

#include <chrono>
#include <functional>

namespace ctvty {
  namespace event {
    class DelayedAction {
    private:
      std::chrono::milliseconds	time, repeat;
      std::function<void()>	callback;

      std::chrono::time_point<std::chrono::high_resolution_clock>	time_start;

      bool			do_repeat;

    private:
      bool			has_finished;

    public:
      DelayedAction(std::function<void()> callback, float delay, float repeat = 0);
      ~DelayedAction();

    public:
      void			Refresh();

    public:
      bool			HasFinished();
    };

    template<template<typename, typename> class container>
    class DelayedActionContainer {
    private:
      container< DelayedActionContainer*, std::allocator<DelayedActionContainer*> >&	_container;
      DelayedAction*	_action;

    public:
      template<typename ... delayed_parameters>
      DelayedActionContainer(container<DelayedActionContainer*, std::allocator<DelayedActionContainer*>>& _c,
			     std::function<void()> callback, delayed_parameters ... p)
	: _container(_c), _action(new DelayedAction ([this, callback] () {
	      callback(); if (sizeof...(delayed_parameters) == 1) delete this;
	  }, p...)) {
	
      }

      ~DelayedActionContainer() {
	if (!_action->HasFinished())
	  delete _action;
	_container.remove_if([this] (DelayedActionContainer* oth) { return oth == this; });
      }
    };
  };
};

#endif
