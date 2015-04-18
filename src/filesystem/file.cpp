#include <regex>

#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>

#include "filesystem/file.hh"

namespace filesystem {

  /*
   *Ctor & Dtor
   */
  File::File(const std::string& path)
    : relativepath(path), _realpath("") {
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
    : valid(oth.valid), relativepath(oth.relativepath), _realpath(oth._realpath), name(oth.name) { }

  File::~File() {}

  /*
   *Create The Said File
   */
  void			File::Create() {
    char		__realpath[256];

    if (*this)
      return ;
    FILE* _file = std::fopen(relativepath.c_str(), "w+");
    if (_file == nullptr)
      return ;
    std::fclose(_file);
    realpath(relativepath.c_str(), __realpath);
    valid = true;
    _realpath = __realpath;
  }

  /*
   * boolean operator
   */
  File::operator	bool() const{
    return (valid);
  }
  File::operator	std::string () const {
    return (name);
  }

  /*
   * Getters
   */
  bool			File::IsDirectory() const {
    struct stat info;
    if (!*this)
      return (false);
    stat( _realpath.c_str(), &info );
    return info.st_mode & S_IFDIR;
  }
  std::string		File::GetPath() const{
    return _realpath;
  }

  std::string		File::GetName() const{
    return name;
  }

};
