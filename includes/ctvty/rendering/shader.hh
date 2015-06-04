#ifndef Shader_hh__
# define Shader_hh__

# include <memory>
# include <string>

# include "ctvty/utils/quaternion.hh"

namespace ctvty {
  namespace rendering {
    class Shader {
    public:
      virtual ~Shader() {}

    public:
      virtual void	SetUniform(const std::string& uniform, const utils::Quaternion& value) = 0;
    };
  };
};

#endif
