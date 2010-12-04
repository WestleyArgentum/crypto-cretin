#ifndef CONSOLE_HPP_
#define CONSOLE_HPP_

void CreateConsole( void );
void RemoveConsole( void );

PCHAR* CommandLineToArgvA( PCHAR CmdLine, int *_argc );

#endif

