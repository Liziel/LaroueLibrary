#ifndef TgaDict_hh__
# define TgaDict_hh__

# include "ctvty/assets/police.hh"

# include "serialization/serializable.hh"

namespace user_defined {
  namespace helper {
    
    class TgaDict : public  serialization::Serializable {
    private:
      std::map<char, std::shared_ptr<ctvty::asset::Police> >	dict;

    public:
			TgaDict(const serialization::Archive&);
      void		Serialize(serialization::Archive&) const override;

    public:
      std::shared_ptr< ctvty::asset::Police >	operator [] (const char);
    };

  };
};

#endif
