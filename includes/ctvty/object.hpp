#ifndef Object_hpp__
# define Object_hpp__

# include <list>

# include "ctvty/utils/vector3d.hh"
# include "ctvty/utils/quaternion.hh"
# include "serialization/serializable.hh"

namespace ctvty {

  class Object : public serialization::Serializable {
  protected:
    /*
     * All Object are referenced with a name
     */
    const std::string name;

  protected:
    /*
     * basic Ctor & Dtor 
     */
    Object(const std::string& _name);
    virtual ~Object();

  public:
    /*
     * intern method corrsponding to the Instantiate & Destroy static methodes
     */
    virtual Object*		clone() const = 0;
    virtual void		intern_Destroy() {}

  private:
    /*
     * A List of all instiated Objects
     */
    static std::list<Object*>	Objects;

  protected:
    bool			destroyableOnLoad;
  public:
    /*
     * Prevent the destruction of the this object tree (parents and childs) when loading a new scene
     */
    void			DontDestroyOnLoad();
    virtual bool		IsDestroyableOnLoad() const;

  public:
    /*
     * Destroy will delete the given object, in the given time (or immediatly if no time is given)
     *    ALWAYS call Destroy and never delete because Destroy will unlink all game part
     */
    static void			Destroy(Object* del);
    static void			Destroy(Object* del, float delay);

  public:
    /*
     * Will instantiate an copy of the given object, and place it at the given coordinate
     * Will also active the clone object
     * ex:
     *    GameObject enemy_template; //with its own component, definition, and position
     *	  GameObject enemy_clone = Instantiate(enemy_template);
     *    GameObject enemy_clone_at_center = Instantiate(enemy_template, {0f, 0f}, vector2::ydirection);
     */
    static Object*		Instantiate(Object* copy);
    static Object*		Instantiate(Object* copy,
					    utils::Vector3D position,
					    utils::Quaternion rotation);

  public:
    /*
     * Static research methodes
     *  FindObjectByType will find the first Object corresponding to the type given in the template
     *  FindObjectsByType will create a vector of all Object corresponfing to the given type
     *  ex:
     *   std::vector<Component*> all_components =
		Object::FindObjectsOfType<Component*>();
     */
    template<typename type>
    static type*		FindObjectOfType() {
      for (Object* object : Objects) {
	if (dynamic_cast<type*>(object) != nullptr)
	  return (dynamic_cast<type*>(object));
      }
      return (nullptr);
    }

    template<typename type>
    static std::vector<type*>		FindObjectsOfType() {
      std::vector<type*>	_founds;

      for (Object* object : Objects) {
	if (dynamic_cast<type*>(object) != nullptr)
	  _founds.push_back(dynamic_cast<type*>(object));
      }
      return (_founds);
    }

  };
};

#endif
