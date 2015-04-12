#ifndef GameObject_hh__
# define GameObject_hh__

#include "ctvty/utility.hh"

namespace ctvty {

  class Object {
  protected:
    const std::string name;

  protected:
    Object(const std::string& _name) : name(_name) {
      Objects.push_back(this);
    }

    virtual ~Object() {
      Objects.remove([this] (Object* _comp) -> bool {return _comp == this;});
    }

  public:
    Object() = delete;

  protected:
    virtual Object*		clone() = 0;
    virtual void		intern_Destroy() {}

  private:
    static std::list<Object*>	Objects;

  public:
    static void			Destroy(Object* del, float _time); //will use the delayed action component
    static void			Destroy(Object* del) { del->intern_destroy(); }

  public:
    static Object*		Instantiate(Object* cp) { return cp->clone(); } 
    static Object*		Instantiate(Object* cp, vector2 position, vector2 rotation); // will clone and set the transform

  public:
    template<typename type>
    static Object*		FindObjectOfType() {
      for (Object* object : Objects) {
	if (dynamic_cast<type*>(object) != nullptr)
	  return (object);
      }
      return (nullptr);
    }

    template<typename type>
    static Object*		FindObjectsOfType() {
      std::vector<type*>	_founds;

      for (Object* object : Objects) {
	if (dynamic_cast<type*>(object) != nullptr)
	  _founds.push_back(object);
      }
      return (_founds);
    }

  };
};

#endif
