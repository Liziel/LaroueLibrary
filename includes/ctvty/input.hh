#ifndef Input_hh__
# define Input_hh__

# include <memory>
# include "serialization/serializable.hh"

namespace ctvty {
  class Input  : public serialization::Serializable {
  public:
    static float			GetKeyState(const std::string&);
    static const utils::Vector3D&	GetMousePosition();

  public:
    static void				receiveEvent(const ctvty::Event* const);
    static void				assignInput(const std::string&, const ctvty::Event* const);

  public:
					Input(const serialization::Archive&);
    void				Serialize(serialiation::Archive&) const;

  public:
    std::shared_ptr<Input>&	singleton() {
      static std::shared_ptr<Input> _singleton(nullptr);

      return _singleton;
    }

  public:
    struct configuration : public serialization::Serialize {
      std::string		base_name;
      std::list<std::string>	alternative_base_name;
      enum class type {
	mouseXaxis, mouseYaxis,
	mousex, mousey,
	key,
	button,
      } associated;

    public:
      
    };

  private:
    std::map<std::string, std::shared_ptr<configuration> >	config_map;
  };
  
};

#endif
