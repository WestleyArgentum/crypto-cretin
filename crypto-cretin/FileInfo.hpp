/*!-----------------//------------------------------------------------------------
\file        FileInfo.hpp
\author      Zak Whaley
\par         email: zak.whaley\@gmail.com
\par         DigiPen login: zwhaley
\par         Course: GAM350 A
\par         Language: C++, Microsoft Visual Studio 2005
\par         Platform: Windows XP
\date        07/06/09

(c) Copyright 2010, DigiPen Institute of Technology (USA). All rights reserved.
    Reproduction or disclosure of this file or its contents without the prior
    written consent of DigiPen Institute of Technology is prohibited.
------------------------------------------------------------------------------*/
#ifndef FILE_INFO_HPP
#define FILE_INFO_HPP

#include <string>

namespace Tools
{

  class FileInfo
  {
  public:
    FileInfo(const std::string &iPath);

    const std::string & FullPath() const;
    const std::string & Directory() const;
    const std::string & FileName() const;
    const std::string & Extension() const;

    //File name with extension:
    const std::string & FileNameOnDisk() const;

  private:
    std::string mFullPath;
    std::string mDirectory;
    std::string mFileName;
    std::string mExtension;
    std::string mFileNameOnDisk;
  };

}

#endif
