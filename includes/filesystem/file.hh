#ifndef File_hh__
# define File_hh__

# include <string>
# include <fstream>

namespace filesystem {
  class File {
  protected:
    bool		valid;
    std::string		relativepath;
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
    bool		IsDirectory() const;

  public:
    void		Create();

  public:
    /*
     * getPath() -> return the real path to the file
     */
    virtual std::string		GetPath() const;
    std::string		GetName() const;
    operator		std::string() const;
  };
};

#endif
