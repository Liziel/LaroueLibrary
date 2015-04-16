#ifndef Serial_hh__
# define Serial_hh__

# include <type_traits>
# include <typeinfo>

# include <fstream>

# include <list>
# include <map>

namespace serialization {
  class Serializable;

  template<typename _type, class _enable = void>
  struct serial_info;

  /*
   * with:
   *  get(serial::interface*)
   *  is(serial::interface*)
   *  make(_type)
   */

  namespace serial {
    class interface {
    public:
      virtual std::string	Stringify(int level = 0) = 0;
      virtual ~interface()	{}
    };
    class string;
    class object;
    class list;
    class integer;
    class floating;
    class boolean;
  };

  class Serial {
  private:
    serial::interface*		serial;

  public:
    template<typename _type>
    const Serial&	operator & (_type& variable) const {
      if (!is<_type>())
	return *this;
      serial_info<_type>::set(serial, variable);
      return *this;
    }

    template<typename _type>
    Serial&		operator & (const _type& variable) {
      if (serial != nullptr)
	delete serial;
      serial = serial_info<_type>::make(variable);
      return *this;
    }

    template<typename _type>
    Serial&		operator & (_type& variable) {
      if (serial != nullptr)
	delete serial;
      serial = serial_info<_type>::make(variable);
      return *this;
    }

    template<typename _type>
    bool		is() const { return serial_info<_type>::is(serial); }

    template<typename _type>
    _type		as() const { return serial_info<_type>::get(serial); }

  public:
    static bool		isBlank(std::string::const_iterator& cursor, std::string::const_iterator end);
    static bool		isDelimiteur(std::string::const_iterator& cursor, std::string::const_iterator end);
    static bool		isSeparateur(std::string::const_iterator& cursor, std::string::const_iterator end);

  public:
    std::string		Stringify(int level = 0) { return serial->Stringify(level); }

  public:
    Serial(serial::interface* = nullptr);
    ~Serial();

  public:
    static Serial*	Instantiate(std::string::const_iterator& current, std::string::const_iterator end);
    static Serial*	InstantiateFromString(const std::string& s) {
      std::string::const_iterator b = s.begin();
      return Instantiate(b, s.end());
    }
    static Serial*	InstantiateFromFile(const std::string& s) {
      std::ifstream		_file;
      _file.open(s);
      if (!_file)
	return nullptr;
      return (InstantiateFromString(std::string(std::istreambuf_iterator<char>(_file),
						std::istreambuf_iterator<char>())));
    }
  public:
    template<typename _type>
    static Serial*	Instantiate(_type _variable) {
      return (new Serial(serial_info<_type>::make(_variable)));
    };
  };

  namespace serial {
    class string : public interface {
    public:
      std::string				Stringify(int level = 0);

    private:
      std::string				_serialized_string;

    public:
      std::string				get() { return _serialized_string; }

    public:
      static std::string			makeString(std::string::const_iterator& cursor, std::string::const_iterator end);
      static bool				isString(std::string::const_iterator& cursor, std::string::const_iterator end);

    public:
      string(std::string::const_iterator& cursor, std::string::const_iterator end);
      string(const std::string& _variable);
      ~string();
    };

    class object : public interface {
    public:
      std::string				Stringify(int level = 0);

    private:
      std::map< std::string, Serial* >		_serialized_object;

    public:
      static bool				isObject(std::string::const_iterator& cursor, std::string::const_iterator end);

    public:
      bool					exist(const std::string& _index) const {
	return _serialized_object.find(_index) != _serialized_object.end();
      }

    public:
      Serial&					operator[] (const std::string& _index);
      const Serial&				operator[] (const std::string& _index) const;

    public:
      void					emplace(const std::string& _, Serial* __) {
	_serialized_object.emplace(_, __);
      }

    private:
      std::string				getClassNameByTypeId(const std::string&);

    public:
      object();
      ~object();
      object(std::string::const_iterator& cursor, std::string::const_iterator end);
      template<typename serializable>
      object(serializable* _s) {
	_s->Serialize(*this);
      }
    };

    class list : public interface {
    public:
      std::string				Stringify(int level = 0);
      template<typename iterator>
      void					Fill(iterator begin, iterator end);

    private:
      std::list<Serial*>			_serialized_list;

    public:
      static bool				isList(std::string::const_iterator& cursor, std::string::const_iterator end);

    public:
      std::list<Serial*>::iterator		begin() { return _serialized_list.begin(); }
      std::list<Serial*>::iterator		end() { return _serialized_list.end(); }

    public:
      ~list();
      list(std::string::const_iterator& cursor, std::string::const_iterator end);
      template<typename iterator>
      list(iterator begin, iterator end) {
	for (; begin != end; ++begin) {
	  _serialized_list.push_back(Serial::Instantiate(*begin));
	}
      }
    };

    class integer : public interface {
    public:
      std::string				Stringify(int level = 0);
      template<typename _integer_>
      void					Fill(_integer_&);

    private:
      long					_serialized_integer;

    public:
      long					get() { return _serialized_integer; }

    public:
      static bool				isInteger(std::string::const_iterator& cursor, std::string::const_iterator end);

    public:
      integer(std::string::const_iterator& cursor, std::string::const_iterator end);
      template<typename _integer_>
      integer(_integer_ _integer) : _serialized_integer(_integer) {}
    };

    class floating : public interface {
    public:
      std::string				Stringify(int level = 0);
      template<typename floating>
      void					Fill(floating&);

    private:
      double					_serialized_floating;

    public:
      double					get() { return _serialized_floating; }

    public:
      static bool				isFloating(std::string::const_iterator& cursor, std::string::const_iterator end);

    public:
      floating(std::string::const_iterator& cursor, std::string::const_iterator end);
      template<typename _floating_>
      floating(_floating_ _floating) : _serialized_floating(_floating) {}
    };

    class boolean : public interface {
    public:
      std::string				Stringify(int level = 0);
      void					Fill(bool&);

    private:
      bool					_serialized_boolean;

    public:
      static bool				isBoolean(std::string::const_iterator& cursor, std::string::const_iterator end);

    public:
      boolean(std::string::const_iterator& cursor, std::string::const_iterator end);
      boolean(bool);
    };

  };

  /*
   * String serial_info
   */
  template<typename _type>
  typename std::enable_if< std::is_same<std::string, _type>::value, _type >::type
		string_get(serial::interface* _interface) {
    serial::string*	_object = dynamic_cast<serial::string*>(_interface);
    return (_object->get());
  }

  template<typename _type>
  typename std::enable_if< std::is_same<char*, typename std::remove_cv<_type>::type >::value, _type >::type
		string_get(serial::interface* _interface) {
    serial::string*	_object = dynamic_cast<serial::string*>(_interface);
    return (_object->get());
  }

  template<typename _type>
  struct serial_info< _type,
		      typename std::enable_if< std::is_constructible<std::string, _type>::value && !std::is_const<_type>::value >::type > {
    using type = serial::string;
    static _type get(serial::interface* _interface) { return string_get<_type>(_interface); }
    static bool	 is(serial::interface* _interface) { return dynamic_cast<serial::string*>(_interface) != nullptr; }
    static void  set(serial::interface* _interface, _type& _variable) { _variable = get(_interface); }
    static serial::interface* make(_type _variable) {
      return new serial::string(_variable);
    }
  };

  template<typename _type>
  struct serial_info< _type,
		      typename std::enable_if< std::is_same< const std::string, typename std::remove_reference<_type>::type >::value >::type > {
    using type = serial::string;
    static _type get(serial::interface* _interface) { return string_get<_type>(_interface); }
    static bool	 is(serial::interface* _interface) { return dynamic_cast<serial::string*>(_interface) != nullptr; }
    static serial::interface* make(_type _variable) {
      return new serial::string(_variable);
    }
  };

  /*
   * Object serial_info, (on Serializable *)
   */
  Serializable*	SerializableInstantiate(serial::object& serial);
  template<typename _type>
  struct serial_info< _type,
		      typename std::enable_if< std::is_assignable<Serializable*&, _type>::value >::type > {
    using type = serial::object;
    static _type get(serial::interface* _interface) {
      return dynamic_cast<_type> (SerializableInstantiate(*(dynamic_cast<serial::object*>(_interface))));
    }
    static bool  is(serial::interface* _interface) { return dynamic_cast<serial::object*>(_interface) != nullptr; }
    static void	 set(serial::interface* _interface, _type& _variable) { _variable = get(_interface); }
    static serial::interface* make(_type _variable) { return new serial::object(_variable); }
  };

  /*
   * List serial_info, for stl container
   */
  template<typename _type, typename _pass>
  struct _do_pass { using type = _pass; };
  template<typename _type>
  struct serial_info < _type,
		       typename _do_pass<typename serial_info< typename _type::value_type >::type,
					 typename std::enable_if< !std::is_constructible<std::string, _type>::value >::type >::type > {
    using type = serial::list;
    static _type get(serial::interface* _interface) {
      _type	_list;

      std::for_each(dynamic_cast<serial::list*>(_interface)->begin(), dynamic_cast<serial::list*>(_interface)->end(),
		    [&_list] (const Serial* serial) {
			_list.emplace_back(serial->as<typename _type::value_type>());
		    });
      return _list;
    }
    static bool  is(serial::interface* _interface) { return dynamic_cast<serial::list*>(_interface) != nullptr; }
    static void	 set(serial::interface* _interface, _type& _variable) { _variable = get(_interface); }
    static serial::interface* make(_type _variable) { return new serial::list(_variable.begin(), _variable.end()); }
  };

  /*
   * Integer serial_info
   */
  double	double_get(serial::interface*);
  long		int_get(serial::interface*);
  double	double_is(serial::interface*);
  long		int_is(serial::interface*);
  template<typename _type>
  struct serial_info< _type,
		      typename std::enable_if< std::is_integral< _type >::value >::type > {
    using type = serial::integer;
    static _type get(serial::interface* _interface) {
      if (dynamic_cast<serial::integer*>(_interface) == nullptr &&
	  dynamic_cast<serial::floating*>(_interface) != nullptr)
	return double_get(_interface);
      return dynamic_cast<serial::integer*>(_interface)->get();
    }
    static bool  is(serial::interface* _interface) {
      return double_is(_interface) ||
	dynamic_cast<serial::integer*>(_interface) != nullptr;
    }
    static void	 set(serial::interface* _interface, _type& _variable) { _variable = get(_interface); }
    static serial::interface* make(_type _variable) { return new serial::integer(_variable); }
  };

  /*
   * Floating serial_info
   */
  template<typename _type>
  struct serial_info< _type,
		      typename std::enable_if< std::is_floating_point< _type >::value >::type > {
    using type = serial::floating;
    static _type get(serial::interface* _interface) {
      if (dynamic_cast<serial::floating*>(_interface) == nullptr &&
	  dynamic_cast<serial::integer*>(_interface) != nullptr)
	return int_get(_interface);
      return dynamic_cast<serial::floating*>(_interface)->get();
    }
    static bool  is(serial::interface* _interface) {
      return dynamic_cast<serial::integer*>(_interface) != nullptr ||
	dynamic_cast<serial::floating*>(_interface) != nullptr;
    }
    static void	 set(serial::interface* _interface, _type& _variable) { _variable = get(_interface); }
    static serial::interface* make(_type _variable) { return new serial::floating(_variable); }
  };

  /*
   * Serials info
   */
  template<typename _type>
  struct serial_info< _type,
		      typename std::enable_if< std::is_assignable<serial::interface*&,
								  typename std::remove_reference< typename std::remove_cv< _type >::type >::type >::value >::type > {
    static _type get(serial::interface* _interface) {
      return dynamic_cast<_type>(_interface);
    }
    static _type is(serial::interface* _interface) {
      return dynamic_cast<_type>(_interface) != nullptr;
    }
  };

  /*
   * Serials info
   */
  template<typename _type>
  struct serial_info< _type,
		      typename std::enable_if< std::is_assignable<serial::interface*&,
								  typename std::remove_cv<
								    typename std::remove_reference< _type >
								    ::type >
								  ::type * >
					       ::value >
		      ::type > {
    static typename std::remove_reference<_type>::type& get(serial::interface* _interface) {
      return * dynamic_cast< typename std::remove_reference<_type>::type *>(_interface);
    }
    static bool is(serial::interface* _interface) {
      return * dynamic_cast<_type*>(_interface) != nullptr;
    }
  };

};

#endif
