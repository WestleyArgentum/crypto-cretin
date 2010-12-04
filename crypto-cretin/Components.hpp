#pragma once

#include "Window.hpp"     // IWindowComponent

struct ComponentInfo
{
  ComponentInfo( DWORD style, unsigned posx, unsigned posy, unsigned width, unsigned height );

  DWORD style_;
  unsigned posx_;
  unsigned posy_;
  unsigned width_;
  unsigned height_;
};    // struct ComponentInfo


class Textbox : public IWindowComponent
{
public:
  Textbox( const std::string &name, const ComponentInfo &info );
  virtual HWND Create( HWND parent, HINSTANCE hInstance );
  virtual void Init( void );

  void SetTextLimit( unsigned limit );
  void SetText( const std::string &str );

  std::string GetText( void );
  void AddText( const std::string &str );

  void Clear( void );

private:
  static LRESULT CALLBACK EditProc( HWND hWndEdit, UINT msg, WPARAM wParam, LPARAM lParam );

private:
  std::string name_;
  ComponentInfo info_;
  unsigned limit_;

  HWND handle_;
  WNDPROC wndProc_;
};  // Textbox


class Button : public IWindowComponent
{
public:
  Button( const std::string &name, int id, const ComponentInfo &info );
  virtual HWND Create( HWND parent, HINSTANCE hInstance );

private:
  std::string name_;    // Button name.
  ComponentInfo info_;
  int id_;

  HWND handle_;
};  // Button

