#ifndef Application_hh__
# define Application_hh__

# include <list>

# include "ctvty/gameObject.hpp"

# include "ctvty/assets/assets.hh"

# include "ctvty/event/delayed.hh"

# include "serialization/serializable.hh"

namespace ctvty {
  
  namespace debug {
    template<typename T>
    decltype(std::declval< typename serialization::serial_info<T>::type >(),
	     bool())	CompressedLog(T t, bool b = true) {
      serialization::Serial json;
      json & t;
      std::cerr << json.CompactStringify();
      return true;
    }

    template<typename T>
    decltype(std::declval< typename serialization::serial_info<T*>::type >(), bool())	CompressedLog(T&t, bool b = true) {
      serialization::Serial json;
      json & &t;
      std::cerr << json.CompactStringify();
      if (b)
	std::cerr << std::endl;
      return true;
    }

    template<typename T, typename ... V>
    void		CompressedLogs(V ... v);

    template<typename T>
    void		CompressedLogs(T t) {
      CompressedLog(t, false);
      std::cerr << std::endl;
    }

    template<typename T, typename T2, typename ... V>
    void		CompressedLogs(T t, T2 t2, V ... v) {
      CompressedLog(t, false);
      std::cerr << ", ";
      CompressedLogs(t2, v...);
    }

    template<typename T>
    decltype(std::declval< typename serialization::serial_info<T>::type >(),
	     bool())	Log(T t, bool b = true) {
      serialization::Serial json;
      json & t;
      std::cerr << json.Stringify();
      return true;
    }

    template<typename T>
    decltype(std::declval< typename serialization::serial_info<T*>::type >(), bool())	Log(T&t, bool b = true) {
      serialization::Serial json;
      json & &t;
      std::cerr << json.Stringify();
      if (b)
	std::cerr << std::endl;
      return true;
    }

    template<typename T, typename ... V>
    void		Logs(V ... v);

    template<typename T>
    void		Logs(T t) {
      Log(t, false);
      std::cerr << std::endl;
    }

    template<typename T, typename T2, typename ... V>
    void		Logs(T t, T2 t2, V ... v) {
      Log(t, false);
      std::cerr << std::endl;
      Logs(t2, v...);
    }

  };
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

  public:
    ~Application();
    Application(const serialization::Archive&);
    void			Serialize(serialization::Archive&) const override;

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
