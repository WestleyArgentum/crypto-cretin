//=======================================================\\

/*!

      @file        Components.cpp

      @author      Robert Onulak
 
      @brief
          Implementation of open and save file dialog boxes.

    Copyright © 2009 DigiPen (USA) Corporation, All Rights Reserved
    Reproduction or disclosure of this file or its contents without 
    the prior written consent of DigiPen Institute of Technology is prohibited.

*/

//========================================================\\

#include "Components.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// ComponentInfo Methods


//**************************************************************************************************
ComponentInfo::ComponentInfo( DWORD style, unsigned posx, unsigned posy, unsigned width,
  unsigned height )
  : style_(style)
  , posx_(posx)
  , posy_(posy)
  , width_(width)
  , height_(height)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Textbox Methods


//**************************************************************************************************
Textbox::Textbox( const std::string &name, const ComponentInfo &info )
  : name_(name)
  , info_(info)
{
}


//**************************************************************************************************
HWND Textbox::Create( HWND parent, HINSTANCE hInstance )
{
  handle_ = CreateWindow(
    "EDIT",
    name_.c_str(),
    info_.style_,                 // Style options for this textbox
    info_.posx_, info_.posy_,     // Textbox position on our window.
    info_.width_, info_.height_,  // Textbox height and width
    parent,                       // Attatch this textbox to the window specified,
    NULL,
    hInstance,                    // Within this application.
    NULL );

  SetTextLimit( 10000 );

  return handle_;
}


//**************************************************************************************************
void Textbox::Init( void )
{
  if ( !( info_.style_ & ES_READONLY ) )
  {
    wndProc_ = (WNDPROC)SetWindowLong( handle_, GWL_WNDPROC, (LPARAM)EditProc );
  }
}


//**************************************************************************************************
void Textbox::SetTextLimit( unsigned limit )
{
  limit_ = limit;

  SendMessage( handle_, EM_SETLIMITTEXT, limit, 0 );
}


//**************************************************************************************************
void Textbox::SetText( const std::string &str )
{
  SendMessage( handle_, WM_SETTEXT, 0, (LPARAM)str.c_str() );
  SendMessage( handle_, WM_VSCROLL, SB_BOTTOM, NULL );
}


//**************************************************************************************************
std::string Textbox::GetText( void )
{
  char *buffer = new char[limit_ + 1];
  std::string str;
  int characters = (int)SendMessage( handle_, WM_GETTEXT, limit_ + 1, (LPARAM)buffer );

  str.assign( buffer, characters );
  delete buffer;

  return str;
}


//**************************************************************************************************
LRESULT CALLBACK Textbox::EditProc( HWND hWndEdit, UINT msg, WPARAM wParam, LPARAM lParam )
{
  Textbox *textbox = Window::GetInstance()->HasComp<Textbox*>( hWndEdit );

  switch ( msg )
  {
    case WM_KEYDOWN:
		  switch (wParam)
		  {
			  case VK_RETURN:
          {
          }
				  break;
		  }
      break;
  }

	return CallWindowProc( textbox->wndProc_, hWndEdit, msg, wParam, lParam );
}


//**************************************************************************************************
void Textbox::AddText( const std::string &str )
{
    // Get the current text within our window so we don't clear it on the client.
  std::string current = GetText();
  unsigned size = static_cast<unsigned>( current.size() + str.size() );

    // If we reached the end of our buffer, start pushing our old data out to make room for the new
    //  data comming in!
  if ( size > limit_ )
  {
    current = current.substr( str.size() );
  }

  current += str;   // Add the current text to the end of this string.

    // Now set the text to have the 'str' passed in added to the textbox.
  SetText( current );
}


//**************************************************************************************************
void Textbox::Clear( void )
{
  SetText( "" );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Button Methods


//**************************************************************************************************
Button::Button( const std::string &name, int id, const ComponentInfo &info )
  : name_(name)
  , info_(info)
  , id_(id)
{
}


//**************************************************************************************************
HWND Button::Create( HWND parent, HINSTANCE hInstance )
{
  handle_ = CreateWindow(
    "BUTTON",
    name_.c_str(),
    info_.style_,                 // Style options for this textbox
    info_.posx_, info_.posy_,     // Textbox position on our window.
    info_.width_, info_.height_,  // Textbox height and width
    parent,                       // Attatch this textbox to the window specified,
    (HMENU)id_,                   // Give this button a particular command id so we can reference it
                                  //  as such when checking for button click.
    hInstance,                    // Within this application.
    NULL );

  return handle_;
}

