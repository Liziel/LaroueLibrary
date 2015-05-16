#ifndef GuiEvent_hh__
# define GuiEvent_hh__

# include <queue>
# include <memory>

# include "ctvty/utils/vector3d.hh"

namespace ctvty {
  
  class Event {
  public:
    enum class Type {
	quit,
	keydown, keyup,
	mousemotion,
	mousebuttondown, mousebuttonup,
    };
  public:
    inline
    enum Type		type() { return _type; }

    /*
     * for mouse button,
     * return which button was pressed
	0 : left
	1 : right
	2 : middle
	3+: undefined (gaming mouse?)
     */
  public:
    inline
    int				button() { return _button; }

    /*
     * for key,
	ascii key code : ascii key
     */
  public:
    inline
    char			character() { return _character; }
    inline
    int				keycode() { return _keycode; }

    /*
     * for mouse motion
     */
  public:
    inline
    const utils::Vector3D&	delta() { return _delta; }

    /*
     * for mouse motion or button
     */
  public:
    inline
    const utils::Vector3D&	position() { return _position; }

  private:
    static std::shared_ptr<Event>&		setcurrent();
  public:
    static const std::shared_ptr<Event>&	current();

  private:
    static std::queue<Event*>	events;
  public:
    static void			Refresh();
    static Event*		Eat();

  private:
    enum Type			_type;

    int				_button;

    char			_character;
    int				_keycode;

    utils::Vector3D		_delta;
    utils::Vector3D		_position;

  public:
    Event() = default;
    Event(char character, int keycode, bool state);
    Event(const utils::Vector3D& delta, const utils::Vector3D& position);
    Event(int button, bool state, const utils::Vector3D& position);
    ~Event() = default;
  };

};

#endif
