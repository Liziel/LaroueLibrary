#include <algorithm>
#include <cctype>
#include <sstream>

#include "serialization/serial.hh"
#include "serialization/serializable.hh"

namespace serialization {
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
    if (serial::object::isObject(cursor, end))
      return (new Serial(new serial::object(cursor, end)));
    if (serial::string::isString(cursor, end))
      return (new Serial(new serial::string(cursor, end)));
    if (serial::list::isList(cursor, end))
      return (new Serial(new serial::list(cursor, end)));
    if (serial::floating::isFloating(cursor, end))
      return (new Serial(new serial::floating(cursor, end)));
    if (serial::integer::isInteger(cursor, end))
      return (new Serial(new serial::integer(cursor, end)));
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

    bool	boolean::isBoolean(std::string::const_iterator& cursor, std::string::const_iterator end) {
      std::string::const_iterator false_test = cursor;
      std::string::const_iterator true_test = cursor;

      if ((std::size_t)std::distance(cursor, end) > sizeof("false")) {
	std::advance(false_test, sizeof("false"));
	if (std::string("false") == std::string(cursor, false_test))
	  return true;
      }

      if ((std::size_t)std::distance(cursor, end) > sizeof("true")) {
	std::advance(true_test, sizeof("true"));
	if (std::string("true") == std::string(cursor, true_test))
	  return true;
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

    integer::			integer(std::string::const_iterator& cursor, std::string::const_iterator) {
      std::string::const_iterator	beg = cursor;

      while (std::isdigit(*cursor)) ++ cursor;
      _serialized_integer = std::stoll(std::string(beg, cursor));
    }

    floating::			floating(std::string::const_iterator& cursor, std::string::const_iterator) {
      std::string::const_iterator	beg = cursor;

      while (std::isdigit(*cursor)) ++ cursor;
      ++cursor;//skip dot
      while (std::isdigit(*cursor)) ++ cursor;
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


  };

  /*
   * Stringify
   */
  namespace serial {
    std::string		string::Stringify(int) const {
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

    std::string		integer::Stringify(int) const {
      std::stringstream	ss("");
      ss << _serialized_integer;
      return (ss.str());
    }

    std::string		floating::Stringify(int) const {
      std::stringstream	ss("");
      ss << _serialized_floating;
      return (ss.str());
    }

    std::string		boolean::Stringify(int) const {
      std::stringstream	ss("");
      ss << std::boolalpha << _serialized_boolean;
      return (ss.str());
    }

  };

  /*
   * serial::object definitions
   */
  Serializable*	SerializableInstantiate(serial::object& serial) {
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
      return (*(_serialized_object.at(_index)));
    }
  };

  double	double_get(serial::interface* i) { return serial_info< double >::get(i); }
  long		int_get(serial::interface* i) { return serial_info< long >::get(i); }
  double	double_is(serial::interface* i) { return serial_info< double >::is(i); }
  long		int_is(serial::interface* i) { return serial_info< long >::is(i); }

};
