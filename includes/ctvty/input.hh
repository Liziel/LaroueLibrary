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

  public:
					Input(const serialization::Archive&);
    void				Serialize(serialiation::Archive&) const;

  private:
    

  public:
    std::shared_ptr<Input>&	singleton() {
      static std::shared_ptr<Input> _singleton(nullptr);

      return _singleton;
    }
  };
  
};

#endif
