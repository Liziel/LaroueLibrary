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
      void				Serialize(serialization::Archive&) const override;

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

  private:
    std::string			name;
    std::size_t			windowX, windowY;

  public:
    static inline std::size_t	WindowHeight() { return GetApplication()->windowY; }
    static inline std::size_t	WindowWidth() { return GetApplication()->windowX; }

  public:
    ~Application();
    Application(const serialization::Archive&);
    void			Serialize(serialization::Archive&) const override;

  public:
    static bool			Initialize(const std::string& assets_location);
    static void			Start();

  public:
    /*
     * Quit == exit from programm with no memory leaks, hum hum
     * Quit(float) == deleayed with seconds time quit
     * CancelQuit() == stop the programm from quitting
     */
    static void			CancelQuit();
    static void			Quit();
    static void			Quit(float seconds);

  public:
    /*
     * Allow to access the assets folders
     */
    static asset::Assets&	Assets();

  public:
    /*
     * Allow to load scene saved in the json
     */
    static void			LoadScene(const std::string& name);
    static void			LoadFileAsScene(const std::string& name);
  };
};

#endif
