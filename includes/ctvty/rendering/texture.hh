#ifndef Texture_hh__
# define Texture_hh__

namespace ctvty {
  namespace rendering {

    class Texture {
    public:
      virtual ~Texture() {}

    public:
      virtual void	Bind() = 0;
    };

  };
};

#endif
