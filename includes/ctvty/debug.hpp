#ifndef Debug_hpp__
# define Debug_hpp__

namespace ctvty {
  namespace debug {
    template<typename T, bool b = true>
    decltype(std::declval< typename serialization::serial_info<T>::type >(),
	     bool())	CompressedLog(T t) {
      serialization::Serial json;
      json & t;
      std::cerr << json.CompactStringify();
      if (b)
	std::cerr << std::endl;
      return true;
    }

    template<typename T, bool b = true>
    decltype(std::declval< typename serialization::serial_info<T*>::type >(), bool())	CompressedLog(T&t) {
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
      CompressedLog<T, false>(t);
      std::cerr << std::endl;
    }

    template<typename T, typename T2, typename ... V>
    void		CompressedLogs(T t, T2 t2, V ... v) {
      CompressedLog<T, false>(t);
      std::cerr << ", ";
      CompressedLogs(t2, v...);
    }

    template<typename T>
    decltype(std::declval< typename serialization::serial_info<T>::type >(),
	     bool())	Log(T t, bool b = true) {
      serialization::Serial json;
      json & t;
      std::cerr << json.Stringify();
      if (b)
	std::cerr << std::endl;
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

    template<typename T>
    decltype(std::declval< typename serialization::serial_info<typename T::value_type*>::type >(), bool())	Log(T&t, bool b = true) {
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
};
#endif
