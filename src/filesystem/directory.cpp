#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "filesystem/directory.hh"

namespace filesystem {
  /*
   * Ctor & Dtor
   */
  Directory::		~Directory() {}

  Directory::		Directory(const std::string& _path)
    : Directory(filesystem::File(_path)) { }

  Directory::		Directory(const filesystem::File& file)
    : File(file) {
    DIR*		_directory = nullptr;
    struct dirent*	_file;

    if (this->File::operator bool())
      _directory = opendir(GetPath().c_str());
    if (_directory != nullptr)
      while ((_file = readdir(_directory)) != nullptr) {
	files.emplace(GetPath() + '/' + std::string(_file->d_name));
      }
    valid = this->File::operator bool() && (_directory != nullptr);
    if (valid)
      closedir(_directory);
  }

  /*
   * boolean operators
   */
  Directory::		operator bool () const {
    return (valid);
  }

  /*
   * Getters
   */
  File			Directory::Access(const std::string& file) const { return File(GetPath() + '/' + file); }
  File			Directory::operator + (const std::string& file) const { return Access(file); }

  /*
   * Go
   */
  void			Directory::Create() {
    char		__realpath[256];

    if (*this || this->File::operator bool())
      return ;
    valid = true; File::valid = true;
    mkdir(relativepath.c_str(), S_IRWXU | S_IRWXO | S_IRWXG);
    realpath(relativepath.c_str(), __realpath);
    _realpath = __realpath;

    DIR* _directory = opendir(GetPath().c_str());
    struct dirent*	_file;
    while ((_file = readdir(_directory)) != nullptr) {
	files.emplace(GetPath() + '/' + std::string(_file->d_name));
      }
    valid = this->File::operator bool() && (_directory != nullptr);
    closedir(_directory);    
  }

  /*
   * Iterators
   */
  Directory::iterator		Directory::begin() { return files.begin(); }
  Directory::iterator		Directory::end() {return files.end(); }

};
