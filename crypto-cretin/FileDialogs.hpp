#pragma once

class OpenFileDialog
{
  public:
    OpenFileDialog( HWND hWnd );

      // Opens the 'Open Dialog Box' which prompts the user to select the file to open.
    BOOL OpenFile( void );
    BOOL OpenFile( const std::string &filename );

    void SetDirectory( const std::string &directory );
    void SetRelativeDir( const std::string &relative_dir );

    const char* GetFileName( void );    // Get the name of the file that the user wants to open.
    const char* GetFileTitle( void );

  private:
    OPENFILENAME ofn_;
    HWND hWnd_;
    char filename_[MAX_PATH];

    std::string dir_;
};    // class OpenFileDialog


class SaveFileDialog
{
  public:
    SaveFileDialog( HWND hWnd );

      // Opens the 'Save Dialog Box' which prompts the user to select the directory and filename
      //  for where to save this file at.
    BOOL SaveFile( void );
      // Default the initial filename for the user to the filename given.
    BOOL SaveFile( const std::string &filename );

    void SetDirectory( const std::string &directory );
    void SetRelativeDir( const std::string &relative_dir );

    const char* GetFileName( void );    // Get the name of the file to save.
    const char* GetFileTitle( void );

  private:
    OPENFILENAME ofn_;
    HWND hWnd_;
    char filename_[MAX_PATH];

    std::string dir_;
};    // class SaveFileDialog

