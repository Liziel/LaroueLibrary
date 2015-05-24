#ifndef Serial_hh__
# define Serial_hh__

# include <iostream>

# include <type_traits>
# include <typeinfo>

# include <functional>

# include <fstream>

# include <list>
# include <map>

# include <exception>

# include <memory>

# include <algorithm>

namespace std {
};

namespace serialization {
  namespace error {
    class undefined_type_reference : public std::runtime_error {
    public:
      undefined_type_reference(const std::string& type);
    };

    class undefined_variable_reference : public std::runtime_error {
    public:
      undefined_variable_reference(const std::string& var);
    };

    class backtrace_serial_error : public std::runtime_error {
    public:
      backtrace_serial_error(const std::string& type, const std::string& _what);
    };
  };
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
      virtual std::string	Stringify(int level = 0) const = 0;
      virtual std::string	CompactStringify(int level = 0) const = 0;
      virtual ~interface()	{}
    };
    class string;
    class object;
    class list;
    class integer;
    class floating;
    class boolean;
    class function;
  };

  class Serial;
  template<typename parameter>
  serial::function*		MakeFunction(const std::string& _function_name, parameter serializable);

  void				StoreFunction(const std::string& _function_name,
					      std::function<Serial*(const Serial&)> _function);

  class Serial {
  private:
    serial::interface*		serial;

  public:
    template<typename _type>
    const Serial&	operator & (_type& variable) const {
      if (!is<_type>()) {
	if (serial_info<_type, serial::function>::is(serial))
	  serial_info<_type, serial::function>::set(serial, variable);
	return *this;
      }
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
    bool		is() const {
      return serial_info<_type>::is(serial);
    }

    template<typename _type>
    _type		as() const {
	if (serial_info<_type, serial::function>::is(serial))
	  return serial_info<_type, serial::function>::get(serial);
      return serial_info<_type>::get(serial);
    }

  public:
    static bool		isBlank(std::string::const_iterator& cursor, std::string::const_iterator end);
    static bool		isDelimiteur(std::string::const_iterator& cursor, std::string::const_iterator end);
    static bool		isSeparateur(std::string::const_iterator& cursor, std::string::const_iterator end);

  public:
    std::string		Stringify(int level = 0) const { return serial->Stringify(level); }
    std::string		CompactStringify(int level = 0) const { return serial->CompactStringify(level); }

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
      std::string				Stringify(int level = 0) const;
      std::string				CompactStringify(int level = 0) const;

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
      std::string				Stringify(int level = 0) const;
      std::string				CompactStringify(int level = 0) const;

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
      std::string				Stringify(int level = 0) const;
      std::string				CompactStringify(int level = 0) const;
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

    class map : public interface {
    public:
      std::string				Stringify(int level = 0) const;
      std::string				CompactStringify(int level = 0) const;
      template<typename iterator>
      void					Fill(iterator begin, iterator end);

    private:
      std::list< std::pair<Serial*, Serial*> >		_serialized_map;

    public:
      static bool					isMap(std::string::const_iterator& cursor, std::string::const_iterator end);

    public:
      std::list< std::pair<Serial*, Serial*> >::iterator	begin() { return _serialized_map.begin(); }
      std::list< std::pair<Serial*, Serial*> >::iterator	end() { return _serialized_map.end(); }

    public:
      ~map();
      map(std::string::const_iterator& cursor, std::string::const_iterator end);
      template<typename iterator>
      map(iterator begin, iterator end) {
	for (; begin != end; ++begin) {
	  _serialized_map.emplace_back(new Serial(serial_info< decltype(begin->first) >::make(begin->first)),
				       new Serial (serial_info< decltype(begin->second) >::make(begin->second)));
	}
      }
    };

    class function : public interface {
    public:
      std::string				Stringify(int level = 0) const;
      std::string				CompactStringify(int level = 0) const;
      
    private:
      std::string				_function_name;
      std::function<Serial*(const Serial&)>	_function;
      Serial*					_parameter;

    private:
      Serial*					_clean;
    public:
      void					StoreCleaner(Serial* c) {
	if (_clean != nullptr)
	  delete _clean;
	_clean = c;
      }

    private:
      static std::map< std::string, std::function<Serial*(const Serial&)> >	_saved_functions;

    public:
      static void				StoreFunction(const std::string& _function_name,
							      std::function<Serial*(const Serial&)> _function);

    public:
      Serial*					Execute();

    public:
      static bool				isFunction(std::string::const_iterator& cursor, std::string::const_iterator end);

    public:
      ~function() {if (_clean != nullptr) delete _clean; delete _parameter; }
      function(std::string::const_iterator& cursor, std::string::const_iterator end);
      template<typename parameter>
      function(std::string _function_name, parameter serializable)
	: _function_name(_function_name), _function(_saved_functions[_function_name]), _clean(nullptr) {
	(*_parameter) & serializable;
      }
    };

    class integer : public interface {
    public:
      std::string				Stringify(int level = 0) const;
      std::string				CompactStringify(int level = 0) const;
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
      std::string				Stringify(int level = 0) const;
      std::string				CompactStringify(int level = 0) const;
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
      std::string				Stringify(int level = 0) const;
      std::string				CompactStringify(int level = 0) const;
      void					Fill(bool&);

    private:
      bool					_serialized_boolean;

    public:
      bool					get() { return _serialized_boolean; }

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
  Serializable*	SerializableInstantiate(const serial::object& serial);
  template<typename _type>
  struct serial_info< _type,
		      typename std::enable_if< std::is_assignable<Serializable*&, _type>::value >::type > {
    using type = serial::object;
    static _type get(const serial::object& _serial) {
      return dynamic_cast<_type> (SerializableInstantiate(_serial));
    }
    static _type get(serial::interface* _interface) {
      return dynamic_cast<_type> (SerializableInstantiate(*(dynamic_cast<serial::object*>(_interface))));
    }
    static bool  is(serial::interface* _interface) { return dynamic_cast<serial::object*>(_interface) != nullptr; }
    static void	 set(serial::interface* _interface, _type& _variable) { _variable = get(_interface); }
    static serial::interface* make(_type _variable) { return new serial::object(_variable); }
  };

  template<typename _type>
  struct serial_info< const _type*,
		      typename std::enable_if< std::is_assignable<Serializable*&, _type*>::value >::type > {
    using type = serial::object;
    static const _type* get(serial::interface* _interface) {
      return dynamic_cast<_type*> (SerializableInstantiate(*(dynamic_cast<serial::object*>(_interface))));
    }
    static bool  is(serial::interface* _interface) { return dynamic_cast<serial::object*>(_interface) != nullptr; }
    static serial::interface* make(const _type* _variable) { return new serial::object(_variable); }
  };

  template<typename _type>
  struct serial_info< std::shared_ptr<_type>,
		      typename std::enable_if< std::is_assignable<Serializable*&, _type*>::value >::type > {
    using type = serial::object;
    static std::shared_ptr<_type> get(serial::interface* _interface) {
      return std::shared_ptr<_type> (dynamic_cast<_type*> (SerializableInstantiate(*(dynamic_cast<serial::object*>(_interface)))));
    }
    static bool  is(serial::interface* _interface) { return dynamic_cast<serial::object*>(_interface) != nullptr; }
    static void	 set(serial::interface* _interface, std::shared_ptr<_type>& _variable) { _variable = get(_interface); }
    static serial::interface* make(const std::shared_ptr<_type>& _variable) { return new serial::object(_variable.get()); }
  };

  template<typename _type>
  struct serial_info< std::unique_ptr<_type>,
		      typename std::enable_if< std::is_assignable<Serializable*&, _type*>::value >::type > {
    using type = serial::object;
    static std::unique_ptr<_type> get(serial::interface* _interface) {
      return std::unique_ptr<_type> (dynamic_cast<_type*> (SerializableInstantiate(*(dynamic_cast<serial::object*>(_interface)))));
    }
    static bool  is(serial::interface* _interface) { return dynamic_cast<serial::object*>(_interface) != nullptr; }
    static void	 set(serial::interface* _interface, std::unique_ptr<_type>& _variable) { _variable = get(_interface); }
    static serial::interface* make(const std::unique_ptr<_type>& _variable) { return new serial::object(_variable.get()); }
  };

  /*
   * For Unique ptr
   */
  template<typename _type, typename _pass>
  struct _do_pass { using type = void; };

  template<typename _type>//unique ptr for constructible member
  struct serial_info< std::unique_ptr<_type>
		      , typename _do_pass< typename serial_info< _type >::type, void >::type > {
    using type = typename serial_info< _type >::type;
    static std::unique_ptr<_type> get(serial::interface* _interface) {
      return std::unique_ptr<_type> ( new _type(serial_info< _type>::get(_interface) ) );
    }
    static bool is(serial::interface* _interface) { return serial_info< _type>::is(_interface); }
    static void set(serial::interface* _interface, std::unique_ptr<_type>& _variable) { _variable.reset( new _type(serial_info< _type>::get(_interface) ) ); }
    static serial::interface* make(const std::unique_ptr<_type>& _variable) { return new type (*_variable); }
  };

  template<typename _type>//unique ptr for constructible member
  struct serial_info< std::shared_ptr<_type> 
		      , typename _do_pass< typename serial_info< _type >::type, void >::type > {
    using type = typename serial_info< _type >::type;
    static std::shared_ptr<_type> get(serial::interface* _interface) {
      return std::shared_ptr<_type> ( new _type(serial_info< _type>::get(_interface) ) );
    }
    static bool is(serial::interface* _interface) { return serial_info< _type>::is(_interface); }
    static void set(serial::interface* _interface, std::shared_ptr<_type>& _variable) { _variable.reset( new _type(serial_info< _type>::get(_interface) ) ); }
    static serial::interface* make(const std::shared_ptr<_type>& _variable) { return new type (*_variable); }
  };

  /*
   * List serial_info, for stl container
   */
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
    static serial::interface* make(const _type& _variable) { return new serial::list(_variable.begin(), _variable.end()); }
  };

  /*
   * Map Serial Info
   */
  template<typename _type>
  struct serial_info < _type,
		       typename _do_pass<typename serial_info< typename _type::key_type >::type,
					 typename serial_info< typename _type::mapped_type >::type
					 >::type > {
    using type = serial::map;
    static _type get(serial::interface* _interface) {
      _type	_map;

      std::for_each(dynamic_cast<serial::map*>(_interface)->begin(), dynamic_cast<serial::map*>(_interface)->end(),
		    [&_map] (std::pair<const Serial*, const Serial*> serial) {
		      _map.emplace(std::move(serial.first->as<typename _type::key_type>()),
				   std::move(serial.second->as<typename _type::mapped_type>()));
		    });
      return _map;
    }
    static bool  is(serial::interface* _interface) { return dynamic_cast<serial::map*>(_interface) != nullptr; }
    static void	 set(serial::interface* _interface, _type& _variable) {
      std::for_each(dynamic_cast<serial::map*>(_interface)->begin(), dynamic_cast<serial::map*>(_interface)->end(),
		    [&_variable] (std::pair<const Serial*, const Serial*> serial) {
		      _variable.emplace(std::move(serial.first->as<typename _type::key_type>()),
				   std::move(serial.second->as<typename _type::mapped_type>()));
		    });      
    }
    static serial::interface* make(const _type& _variable) { return new serial::map(_variable.begin(), _variable.end()); }
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
		      typename std::enable_if< std::is_integral< _type >::value
					       && !std::is_same< bool, _type>::value
					       >::type > {
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

  template<typename _type>
  struct serial_info< _type,
		      typename std::enable_if< std::is_enum< _type >::value >::type > {
    using type = serial::integer;
    static _type get(serial::interface* _interface) {
      return static_cast<_type>(dynamic_cast<serial::integer*>(_interface)->get());
    }
    static bool  is(serial::interface* _interface) {
      return dynamic_cast<serial::integer*>(_interface) != nullptr;
    }
    static void	 set(serial::interface* _interface, _type& _variable) { _variable = static_cast<_type>(get(_interface)); }
    static serial::interface* make(_type _variable) { return new serial::integer(static_cast<long>(_variable)); }
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
   * Bool
   */
  template<typename _type>
  struct serial_info< _type,
		      typename std::enable_if< std::is_same< bool, _type >::value >::type > {
    using type = serial::boolean;
    static _type get(serial::interface* _interface) {
      if (dynamic_cast<serial::boolean*>(_interface) == nullptr &&
	  dynamic_cast<serial::integer*>(_interface) != nullptr)
	return double_get(_interface);
      return dynamic_cast<serial::boolean*>(_interface)->get();
    }
    static bool  is(serial::interface* _interface) {
      return dynamic_cast<serial::boolean*>(_interface) != nullptr ||
	dynamic_cast<serial::integer*>(_interface) != nullptr;
    }
    static void	 set(serial::interface* _interface, _type& _variable) { _variable = get(_interface); }
    static serial::interface* make(_type _variable) { return new serial::boolean(_variable); }
  };

  /*
   * Function serial_info
   */
  template<typename _type>
  struct serial_info< _type,
		      serial::function> {
    using type = serial::function;
    static bool	 is(serial::interface* _interface) { return dynamic_cast<serial::function*>(_interface) != nullptr; }
    static _type get(serial::interface* _interface) {
      serial::function* ex = dynamic_cast<serial::function*>(_interface);
      Serial* serial = ex->Execute();
      ex->StoreCleaner(serial);

      return serial->as<_type>();
    }
    static void	 set(serial::interface* _interface, _type& _variable) {
      serial::function* ex = dynamic_cast<serial::function*>(_interface);
      Serial* serial = ex->Execute();

      (*const_cast<const Serial*>(serial)) & _variable;
      delete serial;
    }
    static serial::interface* make(_type _variable) {
      return _variable;
    }
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


  template<typename parameter>
  serial::function*		MakeFunction(const std::string& _function_name, parameter serializable) {
    return new serial::function(_function_name, serializable);
  }
};

#endif
