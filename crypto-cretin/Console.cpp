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


//**************************************************************************************************
// Parses the command line and stores it as an array of characters.
PCHAR* CommandLineToArgvA( PCHAR CmdLine, int *_argc )
{
  PCHAR*  argv;
  PCHAR   _argv;
  ULONG   len;
  ULONG   argc;
  CHAR    a;
  ULONG   i, j;

  BOOLEAN in_QM;
  BOOLEAN in_TEXT;
  BOOLEAN in_SPACE;

  len = strlen(CmdLine);
  i = ( ( len + 2 ) / 2 ) * sizeof(PVOID) + sizeof(PVOID);

  argv = (PCHAR*)GlobalAlloc( GMEM_FIXED, i + ( len + 2 ) * sizeof(CHAR) );

  _argv = (PCHAR)( ( (PUCHAR)argv ) + i );

  argc = 0;
  argv[argc] = _argv;
  in_QM = FALSE;
  in_TEXT = FALSE;
  in_SPACE = TRUE;
  i = 0;
  j = 0;

  while ( a = CmdLine[i] )
  {
    if ( in_QM )
    {
      if ( a == '\"' )
      {
        in_QM = FALSE;
      }
      else
      {
        _argv[j] = a;
        j++;
      }
    }
    else
    {
      switch(a)
      {
      case '\"':
        {
          in_QM = TRUE;
          in_TEXT = TRUE;
          if ( in_SPACE )
          {
            argv[argc] = _argv+j;
            argc++;
          }

          in_SPACE = FALSE;
        }
        break;

      case ' ':
      case '\t':
      case '\n':
      case '\r':
        {
          if ( in_TEXT )
          {
            _argv[j] = '\0';
            j++;
          }

          in_TEXT = FALSE;
          in_SPACE = TRUE;
        }
        break;

      default:
        {
          in_TEXT = TRUE;
          if ( in_SPACE )
          {
            argv[argc] = _argv+j;
            argc++;
          }

          _argv[j] = a;
          j++;
          in_SPACE = FALSE;
        }
        break;
      }
    }

    i++;
  }

  _argv[j] = '\0';
  argv[argc] = NULL;

  (*_argc) = argc;
  return argv;
}

