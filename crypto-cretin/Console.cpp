#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <fstream>

#include "Console.hpp"

namespace
{
//**************************************************************************************************
  void Redirect( FILE *file, const char *mode, HANDLE handle )
  {
    int fileHandle = _open_osfhandle( (long)handle, _O_TEXT );
    FILE *fp = _fdopen( fileHandle, mode );

    *file = *fp;

    setvbuf( file, NULL, _IONBF, 0 );
  }
}


//**************************************************************************************************
void CreateConsole( void )
{
  const WORD MAX_CONSOLE_LINES = 5000;

    // Allocate a console window for this application.
  AllocConsole();

  CONSOLE_SCREEN_BUFFER_INFO coninfo;
  GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &coninfo );

    // Change the size of our buffer to give us a good amount of room for analyzing our output.
  coninfo.dwSize.Y = MAX_CONSOLE_LINES;
  SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), coninfo.dwSize );

  HANDLE output = GetStdHandle( STD_OUTPUT_HANDLE );
  HANDLE input  = GetStdHandle( STD_INPUT_HANDLE );
  HANDLE error  = GetStdHandle( STD_ERROR_HANDLE );

    // Redirect STDOUT, STDIN and STDERR towards our newly allocated console window.
  Redirect( stdout, "w", output );
  Redirect( stdin,  "r", input );
  Redirect( stderr, "w", error );

    // Also direct cout, cin, cerr, clog, etc... to our console window as well.
  std::ios::sync_with_stdio();
}


//**************************************************************************************************
void RemoveConsole( void )
{
  FreeConsole();
}

