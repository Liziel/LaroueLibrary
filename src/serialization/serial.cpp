#include <algorithm>
#include <cctype>
#include <sstream>

#include "serialization/serial.hpp"
#include "serialization/serializable.hh"

namespace serialization {
  /*
   * exception
   */
  namespace error {
    undefined_type_reference::undefined_type_reference(const std::string& type)
      : std::runtime_error("serial error: type \"" + type + "\" is undefined") {}

    undefined_variable_reference::undefined_variable_reference(const std::string& variable)
      : std::runtime_error("serial error: variable \"" + variable + "\" isn't declared in the serial") {}

    backtrace_serial_error::backtrace_serial_error(const std::string& type, const std::string& _what)
      : std::runtime_error("serial error: instanciated from " + type + "\n" + _what) {}
  };

  /*
   * base serial function and ctor
   */
  Serial::	Serial(serial::interface* _interface)
    : serial(_interface) {}

  Serial::	~Serial() {
    delete serial;
  }

  /*
   * From Json : Parse
   */
  Serial*	Serial::Instantiate(std::string::const_iterator& cursor, std::string::const_iterator end) {
    if (Serial::isBlank(cursor, end))
      return nullptr;

    if (serial::boolean::isBoolean(cursor, end))
      return (new Serial(new serial::boolean(cursor, end)));
    else if (serial::map::isMap(cursor, end))
      return (new Serial(new serial::map(cursor, end)));
    else if (serial::object::isObject(cursor, end))
      return (new Serial(new serial::object(cursor, end)));
    else if (serial::string::isString(cursor, end))
      return (new Serial(new serial::string(cursor, end)));
    else if (serial::list::isList(cursor, end))
      return (new Serial(new serial::list(cursor, end)));
    else if (serial::floating::isFloating(cursor, end))
      return (new Serial(new serial::floating(cursor, end)));
    else if (serial::integer::isInteger(cursor, end))
      return (new Serial(new serial::integer(cursor, end)));
    else if (serial::function::isFunction(cursor, end))
      return (new Serial(new serial::function(cursor, end)));
    return nullptr;
  }

  namespace serial {
    bool	string::isString(std::string::const_iterator& cursor, std::string::const_iterator) {
      return *cursor == '"';
    }

    bool	object::isObject(std::string::const_iterator& cursor, std::string::const_iterator) {
      return *cursor == '{';
    }

    bool	list::isList(std::string::const_iterator& cursor, std::string::const_iterator) {
      return *cursor == '[';
    }

    bool	map::isMap(std::string::const_iterator& cursor, std::string::const_iterator) {
      std::string::const_iterator p = cursor;
      ++p;
      return *cursor == '{' && *p == '[';
    }

    bool	floating::isFloating(std::string::const_iterator& cursor, std::string::const_iterator) {
      std::string::const_iterator c = cursor;

      if (*c == '-') ++ c;
      while (std::isdigit(*c)) ++c;
      return *c == '.';
    }

    bool	integer::isInteger(std::string::const_iterator& cursor, std::string::const_iterator) {
      std::string::const_iterator c = cursor;

      if (*c == '-') ++ c;
      return std::isdigit(*c);
    }

    bool	function::isFunction(std::string::const_iterator& cursor, std::string::const_iterator end) {
      std::string::const_iterator c = cursor;

      while (c != end && *c != '(') ++c;
      if (c == end)
	return false;
      return (_saved_functions.find(std::string(cursor, c)) != _saved_functions.end());
    }

    bool	boolean::isBoolean(std::string::const_iterator& cursor, std::string::const_iterator end) {
      std::string::const_iterator false_test = cursor;
      std::string::const_iterator true_test = cursor;

      if ((std::size_t)std::distance(cursor, end) > sizeof("false")) {
	std::advance(false_test, sizeof("false") - 1);
	if (std::string("false") == std::string(cursor, false_test)) {
	  cursor = false_test;
	  return true;
	}
      }

      if ((std::size_t)std::distance(cursor, end) > sizeof("true")) {
	std::advance(true_test, sizeof("true") - 1);
	if (std::string("true") == std::string(cursor, true_test)) {
	  cursor = true_test;
	  return true;
	}
      }

      return false;
    }
  };

  bool		Serial::isBlank(std::string::const_iterator& cursor, std::string::const_iterator end) {
    while (*cursor == ' '  ||
	   *cursor == '\t' ||
	   *cursor == '\n'
	   ) ++ cursor ;
    return cursor == end;
  }

  bool		Serial::isDelimiteur(std::string::const_iterator& cursor, std::string::const_iterator) {
    if (*cursor == ':') {
      ++cursor;
      return true;
    }
    return false;
  }

  bool		Serial::isSeparateur(std::string::const_iterator& cursor, std::string::const_iterator) {
    if (*cursor == ',') {
      ++cursor;
      return true;
    }
    return false;
  }

  /*
   * From Json : Instantiate
   */
  namespace serial {
    std::string			string::makeString(std::string::const_iterator& cursor, std::string::const_iterator) {
      std::string::const_iterator string_beg;

      ++cursor;
      string_beg = cursor;
      while (*cursor != '"') ++cursor;
      {
	std::string _retval(string_beg, cursor);
	++cursor;
	return _retval;
      }
    }

    string::			string(std::string::const_iterator& cursor, std::string::const_iterator end)
      : _serialized_string(makeString(cursor, end)) {}

    string::			string(const std::string& _variable)
      : _serialized_string(_variable) {}

    object::			object(std::string::const_iterator& cursor, std::string::const_iterator end) {
      ++cursor;
      
      Serial::isBlank(cursor, end);
      while (*cursor != '}') {

	Serial::isBlank(cursor, end);
	std::string	key = string::makeString(cursor, end);
	Serial::isBlank(cursor, end);

	Serial::isDelimiteur(cursor, end);

	Serial::isBlank(cursor, end);
	Serial*		value = Serial::Instantiate(cursor, end);
	Serial::isBlank(cursor, end);

	_serialized_object.emplace(key, value);

	Serial::isBlank(cursor, end);
	Serial::isSeparateur(cursor, end);
	Serial::isBlank(cursor, end);
      }
      ++cursor;
    }

    list::			list(std::string::const_iterator& cursor, std::string::const_iterator end) {
      ++cursor;

      Serial::isBlank(cursor, end);
      while (*cursor != ']') {
	Serial::isBlank(cursor, end);
	Serial*		value = Serial::Instantiate(cursor, end);
	Serial::isBlank(cursor, end);

	_serialized_list.push_back(value);

	Serial::isBlank(cursor, end);
	Serial::isSeparateur(cursor, end);
	Serial::isBlank(cursor, end);
      }
      ++cursor;
    }

    map::			map(std::string::const_iterator& cursor, std::string::const_iterator end) {
      std::advance(cursor, 2);

      while (*cursor != ']') {
	Serial::isBlank(cursor, end);
	Serial* key = Serial::Instantiate(cursor, end);
	Serial::isBlank(cursor, end);

	Serial::isDelimiteur(cursor, end);

	Serial::isBlank(cursor, end);
	Serial*		value = Serial::Instantiate(cursor, end);
	Serial::isBlank(cursor, end);

	_serialized_map.emplace_back(key, value);
	
	Serial::isBlank(cursor, end);
	Serial::isSeparateur(cursor, end);
	Serial::isBlank(cursor, end);
      }
      std::advance(cursor, 2);
    }

    function::			function(std::string::const_iterator& cursor, std::string::const_iterator end)
      : _clean(nullptr) {
      std::string::const_iterator	beg = cursor;

      
      Serial::isBlank(cursor, end);
      while (std::isalnum(*cursor)) ++cursor;
      _function = _saved_functions.at(_function_name = std::string(beg, cursor));
      Serial::isBlank(cursor, end);
      ++cursor;
      Serial::isBlank(cursor, end);
      _parameter = Serial::Instantiate(cursor, end);
      Serial::isBlank(cursor, end);
      ++cursor;
      Serial::isBlank(cursor, end);
    }

    integer::			integer(std::string::const_iterator& cursor, std::string::const_iterator end) {
      bool neg = false;

      if (*cursor == '-') {
	neg = true;
	++cursor;
      }
      Serial::isBlank(cursor, end);
      std::string::const_iterator	beg = cursor;
      while (std::isdigit(*cursor)) ++ cursor;
      if (neg)
	_serialized_integer = -std::stoll(std::string(beg, cursor));
      else
	_serialized_integer = std::stoll(std::string(beg, cursor));
    }

    floating::			floating(std::string::const_iterator& cursor, std::string::const_iterator) {
      bool neg = false;

      if (*cursor == '-') {
	neg = true;
	++cursor;
      }
      std::string::const_iterator	beg = cursor;

      while (std::isdigit(*cursor)) ++ cursor;
      ++cursor;//skip dot
      while (std::isdigit(*cursor)) ++ cursor;
      if (neg)
	_serialized_floating = -std::stold(std::string(beg, cursor));
      else
	_serialized_floating = std::stold(std::string(beg, cursor));
    }

    boolean::			boolean(std::string::const_iterator& cursor, std::string::const_iterator end) {
      std::string::const_iterator false_test = cursor;
      std::string::const_iterator true_test = cursor;

      if ((std::size_t)std::distance(cursor, end) > sizeof("false")) {
	std::advance(false_test, sizeof("false"));
	if (std::string("false") == std::string(cursor, false_test)) {
	  _serialized_boolean = false;
	  cursor = false_test;
	}
      }

      if ((std::size_t)std::distance(cursor, end) > sizeof("true")) {
	std::advance(true_test, sizeof("true"));
	if (std::string("true") == std::string(cursor, true_test)) {
	  _serialized_boolean = true;
	  cursor = true_test;
	}
      }
    }
  };

  /*
   * Dtor
   */
  namespace serial {
    string::	~string() {}

    object::	object() {}
    object::	~object() {
      for (std::pair< std::string, Serial* > pair : _serialized_object)
	delete pair.second;
    }

    list::	~list() {
      for (Serial* serial : _serialized_list)
	delete serial;
    }

    map::	~map() {
      for (std::pair< Serial*, Serial* > pair : _serialized_map) {
	delete pair.first;
	delete pair.second;
      }
    }


    boolean::	boolean(bool b)
      : _serialized_boolean(b) {}
  };

  /*
   * Stringify
   */
  namespace serial {
    std::string		string::Stringify(int) const {
      return ('"' + _serialized_string + '"');
    }

    std::string		string::CompactStringify(int) const {
      return ('"' + _serialized_string + '"');
    }

    std::string		object::Stringify(int level) const {
      std::string	_stringified("{\n");
      std::size_t	i = 0;

      std::for_each(_serialized_object.begin(), _serialized_object.end(), [=, &_stringified]
		   (std::pair< std::string, Serial* > pair) {
		      _stringified += std::string(level + 1, '\t') +
			'"' + pair.first + '"'
			+ " : "
			+ pair.second->Stringify(level + 1)
			+ ( (i + 1 < _serialized_object.size()) ? (",\n") : ("\n") );
		    });
      _stringified += std::string(level, '\t') + "}";
      return (_stringified);
    }

    std::string		object::CompactStringify(int) const {
      std::string	_stringified("{");
      std::size_t	i = 0;

      std::for_each(_serialized_object.begin(), _serialized_object.end(), [=, &_stringified]
		   (std::pair< std::string, Serial* > pair) {
		      _stringified += " \"" +
			pair.first + "\" : "
			+ pair.second->CompactStringify()
			+ ( (i + 1 < _serialized_object.size()) ? (", ") : (" ") );
		    });
      _stringified += "}";
      return (_stringified);
    }

    std::string		map::Stringify(int level) const {
      std::string	_stringified("{[\n");
      std::size_t	i = 0;

      std::for_each(_serialized_map.begin(), _serialized_map.end(), [=, &_stringified]
		   (const std::pair< Serial*, Serial* >& pair) {
		      _stringified += std::string(level + 1, '\t') +
			pair.first->Stringify(level + 1)
			+ " : "
			+ pair.second->Stringify(level + 1)
			+ ( (i + 1 < _serialized_map.size()) ? (",\n") : ("\n") );
		    });
      _stringified += std::string(level, '\t') + "]}";
      return (_stringified);
    }

    std::string		map::CompactStringify(int) const {
      std::string	_stringified("{[");
      std::size_t	i = 0;

      std::for_each(_serialized_map.begin(), _serialized_map.end(), [=, &_stringified]
		   (std::pair< Serial*, Serial* > pair) {
		      _stringified += ' ' + 
			pair.first->CompactStringify() + " : "
			+ pair.second->CompactStringify()
			+ ( (i + 1 < _serialized_map.size()) ? (", ") : (" ") );
		    });
      _stringified += "]}";
      return (_stringified);
    }


    std::string		function::Stringify(int level) const {
      return _function_name + '(' + _parameter->Stringify(level + 1) + ')';
    }

    std::string		function::CompactStringify(int level) const {
      return _function_name + '(' + _parameter->CompactStringify(level + 1) + ')';
    }

    std::string		list::Stringify(int level) const {
      std::string	_stringified("[\n");
      std::size_t	i = 0;

      std::for_each(_serialized_list.begin(), _serialized_list.end(), [=, &_stringified]
		   (Serial* serial) {
		      _stringified += std::string(level, '\t')
			+ serial->Stringify(level + 1)
			+ ( (i + 1 < _serialized_list.size()) ? (",\n") : ("\n") );
		    });
      _stringified += std::string(level, '\t') + "]";
      return (_stringified);
    }

    std::string		list::CompactStringify(int level) const {
      std::string	_stringified("[");
      std::size_t	i = 0;

      std::for_each(_serialized_list.begin(), _serialized_list.end(), [=, &_stringified]
		   (Serial* serial) {
		      _stringified += ' '
			+ serial->CompactStringify(level + 1)
			+ ( (i + 1 < _serialized_list.size()) ? (", ") : (" ") );
		    });
      _stringified += "]";
      return (_stringified);
    }

    std::string		integer::Stringify(int) const {
      std::stringstream	ss("");
      ss << _serialized_integer;
      return (ss.str());
    }

    std::string		integer::CompactStringify(int) const {
      std::stringstream	ss("");
      ss << _serialized_integer;
      return (ss.str());
    }

    std::string		floating::Stringify(int) const {
      std::stringstream	ss("");
      ss << _serialized_floating;
      return (ss.str());
    }

    std::string		floating::CompactStringify(int) const {
      std::stringstream	ss("");
      ss << _serialized_floating;
      return (ss.str());
    }

    std::string		boolean::Stringify(int) const {
      std::stringstream	ss("");
      ss << std::boolalpha;
      ss << _serialized_boolean;
      return (ss.str());
    }

    std::string		boolean::CompactStringify(int) const {
      std::stringstream	ss("");
      ss << std::boolalpha;
      ss << _serialized_boolean;
      return (ss.str());
    }

  };

  /*
   * serial::function definitions
   */
  void			StoreFunction(const std::string& _function_name,
				      std::function<Serial*(const Serial&)> _function) {
    serial::function::StoreFunction(_function_name, _function);
  }
  namespace serial {
    void		function::StoreFunction(const std::string& _function_name,
						std::function<Serial*(const Serial&)> _function) {
      _saved_functions.emplace(_function_name, _function);
    }

    Serial*		function::Execute() {
      return _function(*_parameter);
    }
    
    std::map< std::string, std::function<Serial*(const Serial&)> >	function::_saved_functions({});
  };


  /*
   * serial::object definitions
   */
  Serializable*	SerializableInstantiate(const serial::object& serial) {
    return Serializable::Instantiate(serial);
  }
  namespace serial {
    Serial&		object::operator[] (const std::string& _index) {
      if (_serialized_object.find(_index) == _serialized_object.end())
	_serialized_object.emplace(_index, new Serial(nullptr));
      return (*(_serialized_object.at(_index)));
    }

    std::string		object::getClassNameByTypeId(const std::string& typeId) {
      return Serializable::getTypeIdMap()[typeId];
    }


    const Serial&	object::operator[] (const std::string& _index) const {
      if (_serialized_object.find(_index) == _serialized_object.end())
	throw error::undefined_variable_reference(_index);
      if (_serialized_object.at(_index) == nullptr)
	throw error::undefined_variable_reference(_index);
      return (*(_serialized_object.at(_index)));
    }
  };

  double	double_get(serial::interface* i) { return serial_info< double >::get(i); }
  long		int_get(serial::interface* i) { return serial_info< long >::get(i); }
  double	double_is(serial::interface* i) { return serial_info< double >::is(i); }
  long		int_is(serial::interface* i) { return serial_info< long >::is(i); }

};
