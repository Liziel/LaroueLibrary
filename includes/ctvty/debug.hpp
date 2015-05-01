#ifndef Debug_hpp__
# define Debug_hpp__

namespace ctvty {
  namespace debug {

    inline
    bool&		State() {
      static bool _state = true;
      return _state;
    }

    template<typename T, bool b = true>
    decltype(std::declval< typename serialization::serial_info<T>::type >(),
	     bool())	CompressedLog(T t) {
      if (!State())
	return false;
      serialization::Serial json;
      json & t;
      std::cerr << json.CompactStringify();
      if (b)
	std::cerr << std::endl;
      return true;
    }

    template<typename T, bool b = true>
    decltype(std::declval< typename serialization::serial_info<T*>::type >(), bool())	CompressedLog(T&t) {
      if (!State())
	return false;
      serialization::Serial json;
      json & &t;
      std::cerr << json.CompactStringify();
      if (b)
	std::cerr << std::endl;
      return true;
    }

    template<typename T, typename ... V> inline
    bool		CompressedLogs(V ... v);

    template<typename T> inline
    bool		CompressedLogs(T t) {
      if (!State())
	return false;
      CompressedLog<T, false>(t);
      std::cerr << std::endl;
      return true;
    }

    template<typename T, typename T2, typename ... V> inline
    bool		CompressedLogs(T t, T2 t2, V ... v) {
      if (!State())
	return false;
      CompressedLog<T, false>(t);
      std::cerr << ", ";
      CompressedLogs(t2, v...);
      return true;
    }

    template<typename T>
    decltype(std::declval< typename serialization::serial_info<T>::type >(),
	     bool())	Log(T t, bool b = true) {
      if (!State())
	return false;
      serialization::Serial json;
      json & t;
      std::cerr << json.Stringify();
      if (b)
	std::cerr << std::endl;
      return true;
    }

    template<typename T>
    decltype(std::declval< typename serialization::serial_info<T*>::type >(), bool())	Log(T&t, bool b = true) {
      if (!State())
	return false;
      serialization::Serial json;
      json & &t;
      std::cerr << json.Stringify();
      if (b)
	std::cerr << std::endl;
      return true;
    }

    template<typename T>
    decltype(std::declval< typename serialization::serial_info<typename T::value_type*>::type >(), bool())	Log(T&t, bool b = true) {
      if (!State())
	return false;
      serialization::Serial json;

      std::cerr << "[" << std::endl;

      for (auto o : t) {
	json & &o;
	std::cerr << json.Stringify();
      }

      std::cerr << "]" << std::endl;

      if (b)
	std::cerr << std::endl;
      return true;
    }

    template<typename T, typename ... V> inline
    bool		Logs(V ... v);

    template<typename T> inline
    bool		Logs(T t) {
      if (!State())
	return false;
      Log(t, false);
      std::cerr << std::endl;
      return true;
    }

    template<typename T, typename T2, typename ... V> inline
    bool		Logs(T t, T2 t2, V ... v) {
      if (!State())
	return false;
      Log(t, false);
      std::cerr << std::endl;
      Logs(t2, v...);
      return true;
    }
    
    inline
    void		Enable() {
      State() = true;
    }

    inline
    void		Disable() {
      State() = false;
    }

    struct ScopeDisabler {
    private:
      bool	alreadyDisabled;
    public:
      ScopeDisabler() : alreadyDisabled(!State()) { Disable(); }
      ~ScopeDisabler() { if (!alreadyDisabled) Enable(); }
    };
  };
};
#endif
