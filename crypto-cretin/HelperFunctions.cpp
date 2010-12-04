/*!-----------------//------------------------------------------------------------
\file        HelperFunctions.cpp
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
#include <fstream>
#include <io.h>
#include <ctime>
#include <cstdlib>

namespace Tools
{

  void ReplaceInString(std::string &ioString, const std::string &iSequence, const std::string &iReplace)
  {
    //Find the first occurrence of the sequence to replace...
    size_t lPos = ioString.find(iSequence);

    std::string lPrefix;  //The string before the sequence to replace.
    std::string lPostfix; //The string after the sequence to replace.
                          //(Note: postfix will include subsequent occurrences until they are replaced)

    //While occurrences are still found:
    while(std::string::npos != lPos)
    {
      lPrefix  = ioString.substr(0, lPos);                            //

      size_t lNextStartPos = lPos+iSequence.size();                   //

      if(lNextStartPos >= ioString.size())                            //
        lPostfix = "";                                                //
      else                                                            //
        lPostfix = ioString.substr(lNextStartPos, std::string::npos); //

      ioString =  lPrefix + iReplace;                                 //
      
      //Get the next search position:
      size_t lNextSearchPos = ioString.size();                        //
      
      ioString += lPostfix;                                           //

      lPos = ioString.find(iSequence, lNextSearchPos);                //
    }
  }

  bool PathIsReadable(const std::string &iPath)
  {
    return (-1 != _access(iPath.c_str(), 4)) || //read only
           (-1 != _access(iPath.c_str(), 6));   //read and write
  }

  bool PathIsWritable(const std::string &iPath)
  {
    //The following doesn't work for directories. _access only checks attributes for files:
    //return (-1 != _access(iPath.c_str(), 2)) ||  //write only
    //       (-1 != _access(iPath.c_str(), 6));    //read and write

    //This is why I have to attempt to create a temporary file and see what happens.

    Tools::FileInfo lFile(iPath);

    std::ofstream lTestIfCanWrite;
    std::string lBadFilePath = lFile.Directory() + "_YouWontUseThisName_.bad";

    AlwaysAssert(!FileExists(lBadFilePath),
      "A proper temporary file of the name `_YouWontUseThisName_.bad` could not be "
      "generated. Are there other other files (potentially hidden) of this name that "
      "can be removed? This issue should never arrise; if it does, please contact Zak "
      "Whaley (zak.whaley@gmail.com) with this problem. Apologies for the stupid "
      "nature of this error. It's an insufficiency in development on my part.");

    lTestIfCanWrite.open(lBadFilePath.c_str());

    //Well?! Could you open?
    if(lTestIfCanWrite.fail())
      return false;

    //Then clean up, We're goin' to the Wal-Marts!
    lTestIfCanWrite.close();
    ::DeleteFile(lBadFilePath.c_str());

    return true;
  }

/*
Inspired from:
http://www.computing.net/answers/programming/c-can-you-check-if-a-file-exists/11411.html
*/

  bool FileExists(const std::string &iPath)
  {
    //Old method:
    //std::ifstream lFile;
    //
    //lFile.open(iPath.c_str(), std::ifstream::in);
    //lFile.close();

    //return !lFile.fail();

    //New method:
    int lMode = 0; //Check existance only.
    return 0 == _access(iPath.c_str(), lMode);
  }

  bool PathExists(const std::string &iPath)
  {
    int lMode = 0; //Check existance only.
    return 0 == _access(iPath.c_str(), lMode);
  }

  std::string RemoveExtension(const std::string &iFile)
  {
    size_t lStartOfExtension = iFile.find_last_of(".");

    return iFile.substr(0, lStartOfExtension);
  }

} //Tools