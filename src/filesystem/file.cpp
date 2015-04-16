#include <cstdlib>
#include <regex>
#include "filesystem/file.hh"

namespace filesystem {

  /*
   *Ctor & Dtor
   */
  File::File(const std::string& path)
    : _realpath("") {
    char		__realpath[256];

    if (realpath(path.c_str(), __realpath) != nullptr) {
      valid = true;
      _realpath = __realpath;
    } else
      valid = false;

    {
      std::regex		regex("(.*/)*(.*)", std::regex_constants::extended);
      std::smatch		match;

      std::regex_match(path, match, regex);
      name = match[match.size() - 1];
    }
  }

  File::File(const File& oth)
    : valid(oth.valid), _realpath(oth._realpath), name(oth.name) { }

  File::~File() {}

  
  /*
   * boolean operator
   */
  File::operator	bool() const{
    return (valid);
  }


  /*
   * Getters
   */
  std::string		File::GetPath() const{
    return _realpath;
  }

  std::string		File::GetName() const{
    return name;
  }

};
