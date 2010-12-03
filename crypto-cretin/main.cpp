#include "Window.hpp"
#include "FileDialogs.hpp"

#include "encrypt.hpp"

enum ButtonIDs
{
  BID_OpenFile = 100
};  // enum ButtonIDs


//**************************************************************************************************
LRESULT CALLBACK WinProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  HDC hdc;
  int id;

  std::string str;

  switch ( message )
  {
  case WM_PAINT:
    hdc = BeginPaint( hWnd, &ps );
    EndPaint( hWnd, &ps );
    break;

  case WM_DESTROY:
    PostQuitMessage( 0 );
    break;

  case WM_KEYDOWN:
    switch ( wParam )
    {
    case VK_RETURN:
      break;
    }
    break;

  case WM_COMMAND:
    if ( id = LOWORD( wParam ) )
    {
      switch ( id )
      {
        // Open up the filename given:
      case BID_OpenFile:
        {
        }
        break;
      }
    }
    else
    {
      switch ( wParam )
      {
      case BN_CLICKED:
        std::cout << "Unknown button pressed: #" << GetDlgCtrlID( (HWND)lParam ) << std::endl;
        break;
      }
    }
    break;

  case WM_DROPFILES:
    {
        // Query to see how many files were dropped onto our application window!
      unsigned count = DragQueryFile( (HDROP)wParam, 0xFFFFFFFF, 0, 0 );

        // If at least one was dropped, process only one file to encrypt or decrypt
      if ( count )
      {
        char buffer[MAX_PATH + 1] = {0};

        DragQueryFile( (HDROP)wParam, 0, buffer, sizeof(buffer) - 1 );
        DragFinish( (HDROP)wParam );
      }
    }
    break;
  }

  return DefWindowProc( hWnd, message, wParam, lParam );
}


//**************************************************************************************************
int CALLBACK WinMain(HINSTANCE iInstance, HINSTANCE, LPSTR iCommandLine, int iDisplayParameters)
{
    // Create debuging console:
  CreateConsole();

  std::string N = "15641574130333";
  std::string p = "3954889", 
              q = "3954997";

    // Create our window's application:
  Window *main_window = Window::GetInstance();
  main_window->Create( "Crypto-cretin", WinProc, iDisplayParameters, "Crypto-cretin", 800, 600 );

  OpenFileDialog dialog_box( main_window->GetHwnd() );

  dialog_box.SetRelativeDir( "." );
  if ( dialog_box.OpenFile() )
  {
    printf( "Opened file: %s\n", dialog_box.GetFileName() );
  }

  std::ifstream in("test.txt", std::ios::binary | std::ios::in);
  std::ofstream out("test_enc.txt");

  Encrypt(out, in, mpz_class(N).get_mpz_t(), mpz_class("5").get_mpz_t());

  std::ifstream in2("test_enc.txt", std::ios::in);
  std::ofstream out2("test_dec.txt", std::ios::binary | std::ios::out);

  Decrypt(out2, in2, mpz_class(N).get_mpz_t(), mpz_class("5").get_mpz_t());

    // Run the application, while the window is running:
  while ( main_window->Run() )
  {
  }

  return main_window->ReturnCode();
}