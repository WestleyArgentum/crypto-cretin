//=======================================================\\

/*!

      @file        Debug.hpp

      @author      Chris Schell
 
      @brief
          Useful debugging macros

	  Copyright © 2009 DigiPen (USA) Corporation, All Rights Reserved
    Reproduction or disclosure of this file or its contents without 
    the prior written consent of DigiPen Institute of Technology is prohibited.

*/

//========================================================\\

#ifndef NDEBUG

  #define Assert(x, msg, ...)                                   \
    do{                                                         \
      if(!(x)){                                                 \
        char buffer[250];                                       \
        sprintf_s(buffer, "%s: "##msg##"/n", #x, __VA_ARGS__);  \
        MESSAGE(buffer, "Assert failed");                       \
        __debugbreak();                                         \
      }                                                         \
    }while(0)

  #ifdef _WINDOWS_
    #define MESSAGE(msg, caption) MessageBoxA(0, buffer, caption, MB_OK)
  #else
    #define MESSAGE(msg, caption) std::cout << caption << ": " << buffer << std::endl
  #endif

#else
  #define Assert(x, msg, ...)
  #define MESSAGE(msg, caption)
#endif