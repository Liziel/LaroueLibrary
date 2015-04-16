#ifndef Directory_hh__
# define Directory_hh__

# include <string>
# include <list>

# include "file.hh"

namespace filesystem {
  class Directory {
  public:
    using iterator = std::list<File>::iterator;

  public:
    Directory(const std::string&);
    Directory(const File&);
    ~Directory();

  public:
    Directory(Directory&&) = default;
    Directory(const Directory&) = default;


  public:
    /*
     * Verify the validity of the Diectory
     *   -> ex:
     *	if (filesytem::Directory("toto"))
     *	   std::cout << "toto exist & is a directory" << std::endl;
     */
    operator		bool() const;

  public:
    File		Access(const std::string&) const;
    File		operator + (const std::string&) const;

  public:
    /*
     * getPath() -> return the real path to the file
     */
    std::string		GetPath() const;
    std::string		GetName() const;

  public:
    /*
     * allow to iterate over this directory file list
     */
    iterator		begin();
    iterator		end();

  private:
    bool				valid;
    File				path;
    std::list<File>			files;
  };
};

#endif
