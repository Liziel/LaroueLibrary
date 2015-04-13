#ifndef Serial_hh__
# define Serial_hh__

# include "serialization/archive.hh"

namespace serialization {
  namespace serial {
    class String;
    class Object;
    class List;
    class Integer;
    class FLoating;
    class Boolean;
  };

  class Serial {
  public:
    template<typename _type>
    Serial&		operator & (_type& convertible);

    template<typename _type>
    bool		is() const;

    template<typename _type>
    _type&		as() const;

  public:
    static Serial*	Eat(std::string::iterator& current, std::string::iterator end);
  };

  
};

#endif
