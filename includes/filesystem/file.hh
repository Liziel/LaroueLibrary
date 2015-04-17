#ifndef File_hh__
# define File_hh__

# include <string>
# include <fstream>

namespace filesystem {
  class File {
  private:
    bool		valid;
    std::string		_realpath;
    std::string		name;

  public:
    File(const std::string&);
    File(const File&);
    ~File();

  public:
    File(File&&) = default;

  public:
    /*
     * Verify the validity of the file
     *   -> ex:
     *	if (filesytem::File("toto"))
     *	   std::cout << "toto exist" << std::endl;
     */
    operator bool	() const;

  public:
    /*
     * getPath() -> return the real path to the file
     */
    std::string		GetPath() const;
    std::string		GetName() const;
    operator		std::string() const;
  };
};

#endif
