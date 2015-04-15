#ifndef Event_hh__
# define Event_hh__

#include <vector>
#include <functional>

/*
 * here's what's important in this .hpp
 */
namespace ctvty {
  namespace event {
    namespace parameters {
      class value;
      using values = std::vector<value*>;

      template<typename ... _values>
      values PackValues(_values...);
    };
  };
};

namespace ctvty {
  namespace event {
    
    namespace parameters {
      /*
       * event values packers
       */
      template<typename _type>
      class intern_value;

      class value {
      public:
	virtual std::string	getType() = 0;
	
      public:
	template<typename _type>
	static value*		create(_type _value) {
	  return (new intern_value<_type>(_value));
	}

	template<typename _type>
	_type			as() {
	  return (dynamic_cast< intern_value<_type>* >(this)->get());
	}

	template<typename _type>
	bool			is() {
	  return (dynamic_cast< intern_value<_type>* >(this) != nullptr);
	}
      };

      template<typename _type>
      class intern_value : public value{
      private:
	_type		_value;

      public:
	_type		get() { return _value; }

      public:
	intern_value(_type _v) : _value(_v) {}
      };

      template<typename ... _values>
      values PackValues(_values ... vs) {
	values retval = {
	  (value::create<_values>(vs))...,
	};
	return (retval);
      }

    };

    /*
     * event Receiver
     */
    template<typename _class, typename ... parameters>
    class intern_receiver;

    class receiver {
    public:
      virtual void	operator() (ctvty::event::parameters::values _params) = 0;

    public:
      template<typename _class, typename ... parameters>
      static receiver* create(_class* _this, std::function<void ( _class*, parameters...)> _fn) {
	return (new intern_receiver< _class, parameters ... >(_this, _fn));
      }
    };

    template <int N, typename... T>
    struct typeAt;

    template <typename T0, typename... T>
    struct typeAt<0, T0, T...> {
      using type = typename std::remove_cv< typename std::remove_reference<T0>::type >::type;
    };

    template <int N, typename T0, typename... T>
    struct typeAt<N, T0, T...> {
      using type = typename typeAt<N-1, T...>::type;
    };


    template<unsigned int N>
    struct unfolder;

    template<unsigned int N>
    struct unfolder {
      template<typename _class, typename... args, typename ...final>
      static void apply(ctvty::event::parameters::values	values,
			_class*					_this_,
			std::function<void ( _class*, args... )>fn,
			final ...				sended) {
	if (!values[sizeof ... (args) - N]->is< typename typeAt<sizeof ... (args) - N, args...>::type >())
	  throw std::runtime_error("mismatch type on call");
	unfolder<N - 1>::apply(values, _this_, fn, sended ..., values[sizeof ... (args) - N]->as< typename typeAt<sizeof ... (args) - N, args...>::type >());
      }
    };

    template<>
    struct unfolder <0> {
      template<typename _class, typename ... args, typename ... final>
      static void apply(ctvty::event::parameters::values,
			_class*					_this_,
			std::function<void ( _class*, args... )>fn,
			final ...				sended) {
	fn( _this_, sended ... );
      }
    };


    template<typename _class, typename ... parameters>
    class intern_receiver : public receiver{
    private:
      _class*					_this_;
      std::function<void ( _class*, parameters ... )>	_fn;

    public:
      intern_receiver(_class* _this, std::function<void( _class*, parameters... )> fn)
	: _this_(_this), _fn(fn) {}

    public:
      void operator() (ctvty::event::parameters::values _values) {
	if (_values.size() < sizeof ... (parameters))
	  throw std::runtime_error("not enough operands");
	unfolder<sizeof ... (parameters)>::apply(_values, _this_, _fn);
      }
    };

  };
};

#endif
