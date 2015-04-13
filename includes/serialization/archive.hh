#ifndef Archive_hh__
# define Archive_hh__

#include <string>
#include <ostream>

namespace serialization {
  class Serial;
  class Archive {
  private:

  public:
    const	Serial& operator[](const std::string&) const;
		Serial& operator[](const std::string&);

  public:
    std::ostream&	write(std::ostream&);
  };
};

#endif
