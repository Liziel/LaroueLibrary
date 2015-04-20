#ifndef Application_hh__
# define Application_hh__

# include <list>

# include "ctvty/gameObject.hpp"

# include "ctvty/assets/assets.hh"

# include "ctvty/event/delayed.hh"

# include "serialization/serializable.hh"

namespace ctvty {
  
  class Application : public serialization::Serializable {
    static Application*&	GetApplication();
  
  public:
    struct Scene : public serialization::Serializable {
    private:
      std::list <ctvty::GameObject*>	objects;
      std::string			name;

    public:
      ~Scene();
      Scene(const serialization::Archive&);
      void				Serialize(serialization::Archive&);

    public:
      std::string			GetName() const;
      void				Instantiate() const;
    };

  private:
    bool			leave_state;
    event::DelayedAction*	delayed_quit;
    std::list<Scene*>		scenes;
    std::string			base_scene;
    asset::Assets*		assets;


  public:
    ~Application();
    Application(const serialization::Archive&);
    void			Serialize(serialization::Archive&);

  public:
    static bool			Initialize(const std::string& assets_location);
    static void			Start();

  public:
    static void			CancelQuit();
    static void			Quit();
    static void			Quit(float time);

  public:
    static asset::Assets&	Assets();

  public:
    static void			LoadScene(const std::string& name);
  };
};

#endif
