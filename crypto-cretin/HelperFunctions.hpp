/*!-----------------//------------------------------------------------------------
\file        HelperFunctions.hpp
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
#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <stack>

//#define Assert(x, msg)
#define ErrorIf(x, msg)
#define Insist(x, msg)
#define WarnIf(x, msg)
#define AlwaysAssert(x, msg)
#define AlwaysErrorIf(x, msg)
#define AlwaysInsist(x, msg)
#define AlwaysWarnIf(x, msg)

namespace Tools
{  

  bool PathIsReadable(const std::string &iPath);
  bool PathIsWritable(const std::string &iPath);
  bool FileExists(const std::string &iPath);
  bool PathExists(const std::string &iPath);
  std::string RemoveExtension(const std::string &iFile);
  void ReplaceInString(std::string &oString, const std::string &iSequence, const std::string &iReplace);

  template <typename T>
  std::string ToString(const T &iData)
  {
    std::stringstream ss;
    ss << iData;
    return ss.str();
  }

  template <typename T>
  void SafeRelease(T &iObject)	//iterator support
  {
    if(!iObject)
      return;

    iObject->Release();
    iObject = 0;
  }

  template <typename T>
  void SafeDelete(T &iObject)	//iterator support
  {
    delete iObject;
    iObject = 0;
  }


} //Tools

#endif
