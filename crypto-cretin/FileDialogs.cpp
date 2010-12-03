//=======================================================\\

/*!

      @file        FileDialogs.cpp

      @author      Robert Onulak
 
      @brief
          Implementation of open and save file dialog boxes.

    Copyright © 2009 DigiPen (USA) Corporation, All Rights Reserved
    Reproduction or disclosure of this file or its contents without 
    the prior written consent of DigiPen Institute of Technology is prohibited.

*/

//========================================================\\

#include "FileDialogs.hpp"


//**************************************************************************************************
OpenFileDialog::OpenFileDialog( HWND hWnd ) : hWnd_(hWnd)
{
  SecureZeroMemory( &ofn_, sizeof(ofn_) );
  SecureZeroMemory( &filename_, sizeof(filename_) );

    // Get the current directory of our process.
  char current_dir[MAX_PATH];
  GetCurrentDirectory( sizeof(current_dir), current_dir );

  SetDirectory( current_dir );

  ofn_.lStructSize     = sizeof(ofn_);
  ofn_.hwndOwner       = hWnd;
  ofn_.lpstrFilter     = "All Files (*.*)\0*.*\0";
  ofn_.lpstrFile       = filename_;
  ofn_.nMaxFile        = MAX_PATH;
  ofn_.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
}


//**************************************************************************************************
BOOL OpenFileDialog::OpenFile( void )
{
    // Have the user open a file for us to load.
  BOOL result = GetOpenFileName( &ofn_ );

  return result;
}


//**************************************************************************************************
BOOL OpenFileDialog::OpenFile( const std::string &filename )
{
  if ( filename.size() < MAX_PATH )
  {
    strcpy( filename_, filename.c_str() );

    return OpenFile();
  }
  else
  {
    return FALSE;
  }
}


//**************************************************************************************************
void OpenFileDialog::SetDirectory( const std::string &directory )
{
  dir_ = directory;

  ofn_.lpstrInitialDir = dir_.c_str();
}


//**************************************************************************************************
void OpenFileDialog::SetRelativeDir( const std::string &relative_dir )
{
  dir_ = dir_ + ("\\" + relative_dir);

  ofn_.lpstrInitialDir = dir_.c_str();
}


//**************************************************************************************************
const char* OpenFileDialog::GetFileName( void )
{
  return ofn_.lpstrFile;
}

/**************************************************************************************************/
/**************************************************************************************************/
const char* OpenFileDialog::GetFileTitle( void )
{
  return ofn_.lpstrFileTitle;
}


//**************************************************************************************************
SaveFileDialog::SaveFileDialog( HWND hWnd ) : hWnd_(hWnd)
{
  SecureZeroMemory( &ofn_, sizeof(ofn_) );
  SecureZeroMemory( &filename_, sizeof(filename_) );

    // Get the current directory of our process.
  char current_dir[MAX_PATH];
  GetCurrentDirectory( sizeof(current_dir), current_dir );

  SetDirectory( current_dir );

  ofn_.lStructSize     = sizeof(ofn_);
  ofn_.hwndOwner       = hWnd_;
  ofn_.lpstrFilter     = "All Files (*.*)\0*.*\0";
  ofn_.lpstrFile       = filename_;
  ofn_.nMaxFile        = MAX_PATH;
  ofn_.Flags           = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
                         OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
}


//**************************************************************************************************
BOOL SaveFileDialog::SaveFile( void )
{
    // Have the user open a file for us to load.
  BOOL result = GetSaveFileName( &ofn_ );

  return result;
}


//**************************************************************************************************
BOOL SaveFileDialog::SaveFile( const std::string &filename )
{
  if ( filename.size() < MAX_PATH )
  {
    strcpy( filename_, filename.c_str() );

    return SaveFile();
  }
  else
  {
    return FALSE;
  }
}


//**************************************************************************************************
void SaveFileDialog::SetDirectory( const std::string &directory )
{
  dir_ = directory;

  ofn_.lpstrInitialDir = dir_.c_str();
}


//**************************************************************************************************
void SaveFileDialog::SetRelativeDir( const std::string &relative_dir )
{
  dir_ = dir_ + ("\\" + relative_dir);

  ofn_.lpstrInitialDir = dir_.c_str();
}


//**************************************************************************************************
const char* SaveFileDialog::GetFileName( void )
{
  return ofn_.lpstrFile;
}


//**************************************************************************************************
const char* SaveFileDialog::GetFileTitle( void )
{
  return ofn_.lpstrFile;
}

