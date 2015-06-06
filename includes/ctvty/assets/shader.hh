#ifndef AssetShader_hh__
# define AssetShader_hh__

# include "serialization/serializable.hh"

# include "ctvty/rendering/renderer.hh"
# include "ctvty/rendering/shader.hh"

namespace ctvty {
  namespace asset {

    class Shader : public serialization::Serializable{
    public:
      class Wrapper : public serialization::Serializable{
      public:
	Wrapper(const serialization::Archive&);
	void	Serialize(serialization::Archive&) const;

      private:
	std::string					fragment_shader, vertex_shader;
	std::shared_ptr<ctvty::rendering::Shader>	shader;
      public:
	inline std::shared_ptr<ctvty::rendering::Shader>get() { return shader; }
      };
    private:
      std::string		wrapper;
      std::shared_ptr<Wrapper>	_object;

    public:
      inline
      std::shared_ptr<ctvty::rendering::Shader>	GetShader() {
	return _object->get();
      }
      inline
      operator	bool () { return static_cast<bool>(_object); }
    public:
		Shader(const serialization::Archive&);
      void	Serialize( serialization::Archive& ) const;

    public:
      void	delayedInstantiation();
    };
  };
};

#endif
