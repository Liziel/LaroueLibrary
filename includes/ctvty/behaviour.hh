#ifndef Behaviour_hpp__
# define Behaviour_hpp__

# include "ctvty/gameObject.hpp"
# include "ctvty/component.hpp"

namespace ctvty {

  class Behaviour : public Component {
  protected:
    bool	enabled;

  public:
    Behaviour(GameObject* parent,
	      const std::string& name);

  public:
    /*
     * Enable Event Reception,
     * true by default;
     */
    void		SetEnable(bool state);

  public:
    /*
     * An evolution to the DoImplement method to match the enabled/disabled pattern
     */
    virtual bool	DoImplement(const std::string& name);
  };
};

#endif
