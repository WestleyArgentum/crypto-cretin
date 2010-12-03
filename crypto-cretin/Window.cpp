//=======================================================\\

/*!

      @file        Window.cpp

      @author      Robert Onulak
 
      @brief
          Implementation of Microsoft Windows gui

    Copyright © 2009 DigiPen (USA) Corporation, All Rights Reserved
    Reproduction or disclosure of this file or its contents without 
    the prior written consent of DigiPen Institute of Technology is prohibited.

*/

//========================================================\\

#include "Window.hpp"

#include <mmsystem.h>
#include <shellapi.h>   // DragAcceptFiles
#include <commctrl.h>   // InitCommonControls

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Window Public Methods


//**************************************************************************************************
Window* Window::GetInstance( void )
{
  static Window instance;
  return &instance;
}


//**************************************************************************************************
bool Window::Create( const std::string &name, WNDPROC proc, int show,
  const std::string &title, unsigned width, unsigned height )
{
  name_   = name;
  title_  = title;
  width_  = width;
  height_ = height;

    // Get the HINSTANCE of this application. (aka first param of WinMain)
  hInst_ = GetModuleHandle( NULL );

  // WS_BORDER           - Creates a window that has a border.
  // WS_CAPTION          - Creates a window that has a title bar (implies the WS_BORDER style).
  //                       Cannot be used with the WS_DLGFRAME style.
  // WS_DLGFRAME         - Creates a window with a double border but no title.
  // WS_MAXIMIZEBOX      - Creates a window that has a Maximize button.
  // WS_MINIMIZE         - Creates a window that is initially minimized. For use with the
  //                        WS_OVERLAPPED style only.
  // WS_MINIMIZEBOX      - Creates a window that has a Minimize button.
  // WS_OVERLAPPED       - Creates an overlapped window. An overlapped window usually has a caption
  //                        and a border.
  // WS_OVERLAPPEDWINDOW - Creates an overlapped window with the WS_OVERLAPPED, WS_CAPTION,
  //                        WS_SYSMENU, WS_THICKFRAME, WS_MINIMIZEBOX, and WS_MAXIMIZEBOX styles.
  // WS_POPUP            - Creates a pop-up window. Cannot be used with the WS_CHILD style.
  // WS_POPUPWINDOW      - Creates a pop-up window with the WS_BORDER, WS_POPUP, and WS_SYSMENU
  //                        styles. The WS_CAPTION style must be combined with the WS_POPUPWINDOW
  //                        style to make the Control menu visible.
  // WS_THICKFRAME       - Creates a window with a thick frame that can be used to size the window.

  style_ = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

  RECT fullWindow = { 0, 0, width_, height_ };
  AdjustWindowRect( &fullWindow, style_, FALSE );

    // Setup all the variables that make up our window!
  window_.cbSize        = sizeof(WNDCLASSEX);
  window_.style         = CS_CLASSDC;
  window_.lpfnWndProc   = proc;
  window_.cbClsExtra    = 0;
  window_.cbWndExtra    = 0;
  window_.hInstance     = hInst_;
  window_.hIcon         = LoadIcon( hInst_, MAKEINTRESOURCE(IDI_APPLICATION) );
  window_.hCursor       = LoadCursor( NULL, IDC_ARROW );
  window_.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
  window_.lpszMenuName  = NULL;
  window_.lpszClassName = name_.c_str();
  window_.hIconSm       = LoadIcon( hInst_, MAKEINTRESOURCE( IDI_APPLICATION ) );

    // Register this window with the OS
  if ( !RegisterClassEx( &window_ ) )
  {
    return false;
  }

    // The create our window. Bam!
  handle_ = CreateWindow(
    name_.c_str(),                  // Application name.
    title_.c_str(),                 // Title bar text.
    style_,                         // Style of the window we are creating.
    CW_USEDEFAULT, CW_USEDEFAULT,   // Initial position (x, y)
    width_, height_,                // Window size
    GetDesktopWindow(),             // Window's parent.
    NULL,                           // Menu bar.
    hInst_,                         // Application instance.
    NULL );                         // lpParam not used.

    // Window not created...
  if ( handle_ == NULL )
  {
    return false;
  }

    // Accept file drag and drop.
	DragAcceptFiles( handle_, TRUE );

    // The display this window with the presentation parameter ('show_') defined by the client and
    //  start having this window be updated.
  ShowWindow( handle_, show );
  UpdateWindow( handle_ );

  return true;
}


//**************************************************************************************************
void Window::AddComponent( IWindowComponent *component )
{
  if ( component )
  {
    HWND comp = component->Create( handle_, hInst_ );

    if ( comp )
    {
      map_[comp] = component;

      component->Init();
    }
  }
}


//**************************************************************************************************
IWindowComponent* Window::GetComponent( HWND component )
{
  return map_[component];
}


//**************************************************************************************************
bool Window::Run( void )
{
    // Process all messages recieved by the OS for our window's created.
  while ( PeekMessage( &msg_, NULL, 0, 0, PM_REMOVE ) )
  {
    if ( msg_.message == WM_QUIT )
    {
      PostQuitMessage(0);

      return false;
    }

    TranslateMessage( &msg_ );
    DispatchMessage( &msg_ );
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Window Private Methods


//**************************************************************************************************
Window::Window( void ) : width_(0), height_(0), handle_(NULL)
{
  INITCOMMONCONTROLSEX common_controls;
  common_controls.dwSize = sizeof(INITCOMMONCONTROLSEX);
  common_controls.dwICC = ICC_PROGRESS_CLASS;

  InitCommonControlsEx( &common_controls );
}


//**************************************************************************************************
Window::~Window( void ) throw()
{
  if ( handle_ )
  {
    UnregisterClass( name_.c_str(), hInst_ );
  }
}

