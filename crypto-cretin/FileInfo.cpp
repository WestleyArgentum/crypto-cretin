/*!-----------------//------------------------------------------------------------
\file        FileInfo.cpp
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
#include "HelperFunctions.hpp"
#include "FileInfo.hpp"

namespace Tools
{

  FileInfo::FileInfo(const std::string &iPath):
  mFullPath(iPath)
  {
    //Get the extension:
    size_t lExtensionStart =
      iPath.find_last_of(".");

    //Get the file name:
    size_t lFileNameStart = 
      iPath.find_last_of("\\/") + 1;

    //In case there was no extension, and a folder had a period:
    if(lExtensionStart <= lFileNameStart)
      lExtensionStart = std::string::npos;

    //If there was no extension:
    if(std::string::npos != lExtensionStart)
      mExtension = iPath.substr(lExtensionStart);

    //Get the directory path without the filename:
    mDirectory = iPath.substr(0, lFileNameStart);

    size_t lNameLength = mFullPath.size() - mDirectory.size();

    Assert(std::string::npos != lFileNameStart, "Invalid file path.");

    //Get the file name with no extension:
    mFileName = iPath.substr(lFileNameStart, lNameLength - mExtension.size());

    //Get the file name as it appears on disk:
    mFileNameOnDisk = mFileName + mExtension;
  }

  const std::string & FileInfo::FullPath() const
  {
    return mFullPath;
  }

  const std::string & FileInfo::Directory() const
  {
    return mDirectory;
  }

  const std::string & FileInfo::FileName() const
  {
    return mFileName;
  }

  const std::string & FileInfo::Extension() const
  {
    return mExtension;
  }

  const std::string & FileInfo::FileNameOnDisk() const
  {
    return mFileNameOnDisk;
  }

}
