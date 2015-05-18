#ifndef Input_hh__
# define Input_hh__

# include <memory>
# include "serialization/serializable.hh"

# include "ctvty/utils/vector3d.hh"
# include "ctvty/event.hh"

namespace ctvty {
  class Input  : public serialization::Serializable {
  public:
    static float			GetKeyState(const std::string&);
    static const utils::Vector3D&	GetMousePosition();

  public:
    static void				receiveEvent(const ctvty::Event* const);
    static void				AssignInput(const std::string&, const ctvty::Event* const);

  public:
					Input(const serialization::Archive&);
    void				Serialize(serialization::Archive&) const;

  public:
    inline
    static std::shared_ptr<Input>&	singleton() {
      static std::shared_ptr<Input> _singleton(nullptr);

      return _singleton;
    }

  public:
    struct configuration : public serialization::Serializable {
      std::list<std::string>	base_names;//temporary member

      enum class type {
	mouseXaxis, mouseYaxis,
	mousex, mousey,
	key,
	button,
      } associated;
      int			key;
      float			value;

    public:
      void	Serialize(serialization::Archive&) const override;
		configuration(const serialization::Archive&);
		configuration() = default ;
    };

  private:
    std::map<std::string, std::shared_ptr<configuration> >	config_map;
    ctvty::utils::Vector3D					mouse;
  };
  
};

#endif
