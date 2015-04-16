#include <dirent.h>
#include "filesystem/directory.hh"

namespace filesystem {
  /*
   * Ctor & Dtor
   */
  Directory::		~Directory() {}

  Directory::		Directory(const std::string& _path)
    : Directory(filesystem::File(_path)) { }

  Directory::		Directory(const filesystem::File& file)
    : path(file) {
    DIR*		_directory = nullptr;
    struct dirent*	_file;

    if (path)
      _directory = opendir(path.GetPath().c_str());
    if (_directory != nullptr)
      while ((_file = readdir(_directory)) != nullptr) {
	files.emplace_back(GetPath() + '/' + std::string(_file->d_name));
      }
    valid = path && (_directory != nullptr);
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
  File			Directory::Access(const std::string& file) const { return File(path.GetPath() + '/' + file); }
  File			Directory::operator + (const std::string& file) const { return Access(file); }
  std::string		Directory::GetPath() const { return path.GetPath(); }
  std::string		Directory::GetName() const { return path.GetName(); }

  /*
   * Iterators
   */
  Directory::iterator		Directory::begin() { return files.begin(); }
  Directory::iterator		Directory::end() {return files.end(); }

};
