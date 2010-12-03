#pragma once


const unsigned DEFAULT_WIDTH  = 800;
const unsigned DEFAULT_HEIGHT = 600;


struct IWindowComponent
{
  virtual ~IWindowComponent( void ) throw() {;}
  virtual HWND Create( HWND parent, HINSTANCE hInstance ) = 0;

  virtual void Init( void ) {;}
};    // struct IWindowComponent


class Window
{
public:
    /// Get the window singleton:
  static Window* GetInstance( void );

    /// Create yourself a window!
  bool Create( const std::string &name, WNDPROC proc, int show = SW_SHOWNORMAL,
    const std::string &title = "" /*name*/, unsigned width = DEFAULT_WIDTH,
    unsigned height = DEFAULT_HEIGHT );

  HWND GetHwnd( void ) const { return handle_; }

    /// Add UI components to the main window.
  void AddComponent( IWindowComponent *component );
  IWindowComponent* GetComponent( HWND component );

  template < typename ComponentType >
  ComponentType HasComp( HWND component )
  {
    return static_cast<ComponentType>( GetComponent( component ) );
  }

    /// Run the update for the window:
  bool Run( void );
    /// Return code of the window.
  WPARAM ReturnCode( void ) const { return msg_.wParam; }

private:
  Window( void );
  ~Window( void ) throw();

    // Not implemented
  Window( const Window &rhs );
  Window& operator=( const Window &rhs );

private:
  unsigned width_;
  unsigned height_;

  std::string name_;
  std::string title_;

  WNDCLASSEX window_;
  DWORD style_;
  MSG msg_;
  HWND handle_;
  HINSTANCE hInst_;

  typedef std::map< HWND, IWindowComponent* > ComponentMap;
  ComponentMap map_;
};  // Window

  // Include all the window component objects as well...
#include "Components.hpp"

