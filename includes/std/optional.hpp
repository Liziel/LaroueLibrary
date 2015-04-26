#ifndef Optional_hh__
# define Optional_hh__

# include <memory>

namespace ctvstd {

  static const struct		optional_none_t {
  public:
    optional_none_t() {}
  }				none;

  template<typename type>
  class Optional {
  private:
    std::shared_ptr<type>	var;

  public:
    Optional()
      : var(nullptr) {}
    Optional(const Optional& oth)
      : var(oth.var) {}
    Optional(ctvstd::optional_none_t)
      : var(nullptr) {}
    Optional(std::nullptr_t)
      : var(nullptr) {}
    template<typename ... ctor_args>
    Optional(ctor_args ... _args)
      : var(new type ( _args ... )) {}

  public:
		operator bool () { return var.operator bool (); }
    auto	operator * () -> decltype( *var ) { return *(var.get()); }
    auto	operator ->() -> decltype( var.operator->() ) { return var.operator -> (); }

  public:
    Optional&	operator = (const Optional& oth) { var = oth.var; return *this;}
  };

};

#endif
