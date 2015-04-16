#include "ctvty/object.hpp"

namespace ctvty {
  /*
   * Ctor & Dtor
   */
  Object::			Object(const std::string& _name) : name(_name) {
    Objects.push_back(this);
  }

  Object::			~Object() {
    Objects.remove_if([this] (Object* _comp) -> bool {return _comp == this;});
  }

  std::list<Object*>		Object::Objects({});

  /*
   * Destroy
   */
  void				Object::Destroy(Object* del) {
    del->intern_Destroy();
    delete del;
  }
};
