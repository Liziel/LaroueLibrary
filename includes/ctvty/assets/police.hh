#ifndef POLICE_HH__
# define POLICE_HH__

# include <memory>
# include <string>

# include <SDL/SDL_ttf.h>

# include "serialization/serializable.hh"

namespace ctvty {
  namespace asset {

    class Police : public serialization::Serializable {
    public:
      class Wrapper : public serialization::Serializable {
      private:
	std::size_t		size;
	std::string		path;

      private:
	TTF_Font*		police;
      public:
	inline
	TTF_Font&		operator * () { return *police; }
	inline
	TTF_Font&		get() { return **this; }
      public:
	~Wrapper();
			Wrapper(const serialization::Archive&);
	void		Serialize(serialization::Archive&) const;
      };
    private:
      std::string		wrapper;
      std::shared_ptr<Wrapper>	_object;
      
    public:
      inline
      TTF_Font&			GetPolice() {
	return *(*_object);
      }
      inline
      std::shared_ptr<Wrapper>	GetShared() {
	return _object;
      }

    public:
			Police(const serialization::Archive&);
      void		Serialize( serialization::Archive& ) const;

    public:
      inline
      operator		bool() { return (bool) _object; }
      void		delayedInstantiation();
    };

  };
};

#endif
